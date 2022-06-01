#include <Musicon_v2.h>

Musicon_v2::Musicon_v2(){
}
Musicon_v2::~Musicon_v2(){
}

void Musicon_v2::init(LiquidCrystal_I2C* lcd){
    analogReference( INTERNAL );
    pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
    this->_lcd = lcd;
    _m_param = new M_data();
    Serial.begin(57600);
    Wire.begin();
    Wire.setClock(400000);
    //TWBR = 16; // Change the i2c clock to 400KHz
    this->_m_lcd = new M_lcd(_m_param, lcd);
    _m_power = new M_power(_m_param, _m_lcd);
    //_ams5600 = new AMS_5600();
    _m_motor = new M_motor(_m_param);
    _m_param->parameter(PARAM_MOTOR_PERM_TO_MOVE, 0);
    // Serial.print("Adres: ");
    // Serial.println(M_lcd.getAdress());
    // Serial.println(0x27);
    
}
 
void Musicon_v2::main(){
    //_cycleTimeCheck();
    _m_lcd->refresh();
    _m_motor->refresh();
    _m_power->refresh();

    if((millis() % 40) == 0) _menu_sw_control();
    if((_time_order_subroutines + 80) < millis()){
        if(_init_ok){
            //_orderSubroutines(); 
            _m_lcd->screenRefresh(); 
        }
       _time_order_subroutines = millis();
    } 
    if(_init_ok && _setter_hall_init_ok){        
        _clickSetterLimitSwitch();
        _tactCalculate();
    }
    if((_time_mem_hall + 20) < millis()){
        _readMotorHall();
        _readSetterHall(); 
        _time_mem_hall = millis();
    }
    
    if(_m_param->parameter(PARAM_INIT_OK_SETTER)  &&
       _m_param->parameter(PARAM_SETTER_TACT) == _m_param->parameter(PARAM_TACT_MIN_VALUE) && 
       _m_param->parameter(PARAM_SETTER_LIM_SW_STAT) == 2){
           _m_motor->init_ok = true;
    }
    if(_m_param->init_ok){
        _init_ok = true;
    }
    if(_m_param->parameter(PARAM_POWER_STATUS) != 3){
        _m_motor->init_ok = false;
    }
    if((_time_mem_bit1000ms + 1000) < millis()){
        _m_lcd->bit1000ms = !_m_lcd->bit1000ms;
        _time_mem_bit1000ms = millis();
        int time_inactive = _m_param->parameter(PARAM_INACTION_TIME);
        time_inactive++;
        _m_param->parameter(PARAM_INACTION_TIME,time_inactive);
    }
    if(abs(_m_param->parameter(PARAM_MOTOR_VELOCITY)) > 10){
        _m_param->parameter(PARAM_INACTION_TIME,0);
    }
}
void Musicon_v2::_cycleTimeCheck(){
        _count_cycle++;
        _time_act_us = micros();
        unsigned long diff = _time_act_us - _time_cycle_last_us;
        _time_cycle_last_us = _time_act_us;
        if(diff > _time_cycle_max_us){
            _time_cycle_max_us = diff;
        }
        if(diff < _time_cycle_min_us){
            _time_cycle_min_us = diff;
        }
        if(_count_cycle == 1000){
            _m_param->parameter(PARAM_CYCLE_MAX_TIME, (int)_time_cycle_max_us);
            _m_param->parameter(PARAM_CYCLE_MIN_TIME, (int)_time_cycle_min_us);
            _count_cycle = 0;
            _time_cycle_max_us = 0;
            _time_cycle_min_us = 32000;
        }
}
void Musicon_v2::_orderSubroutines(){

    switch (_nextSubroutines)
    {
    case 0:
        _m_lcd->screenRefresh();    // 0
        break;
    case 1:
        // _readSetterHall();          // 25
        break;
    case 2:
        //_readMotorHall();           // 50
        break;


    default:
        break;
    }
    _nextSubroutines++;
    if(_nextSubroutines == 3){
        _nextSubroutines = 0;
    }
}
void Musicon_v2::_readMotorHall(){
    int err = _multiplexer(1);
    if(err > 0) return;
    if(_m_param->parameter(PARAM_I2C_STATUS) == 1 && _ams5600.detectMagnet() == 1 ){
        float motor_angle = _convertRawAngleToDegrees(_ams5600.getRawAngle());
        unsigned long diff_time = micros() - _time_last_read_motor_hall;
        if((_angle_last_read_motor_hall < 500.0 && motor_angle > 3100.0) ||
           (motor_angle < 500.0 && _angle_last_read_motor_hall > 3100.0) ||
           (_angle_last_read_motor_hall == 9999.0) 
          ){
                _angle_last_read_motor_hall = motor_angle;
                _time_last_read_motor_hall = micros();
                return;
           }
        float diff_angle = motor_angle - _angle_last_read_motor_hall;
        if(diff_angle >= 0.0){
            if(_velocity_mem_count < 2) _velocity_mem_count++;
        }else{
            if(_velocity_mem_count > (-2)) _velocity_mem_count--;
        }
        if(_velocity_mem_count > (-2) && _velocity_mem_count < (2)){
            _angle_last_read_motor_hall = motor_angle;
            _time_last_read_motor_hall = micros();
            return;
        }

        _m_param->parameter(PARAM_HALL_MOTOR_DEG, round(motor_angle));

        float diff_angle_abs = diff_angle>0.0?diff_angle:(diff_angle*-1.0);
        if(diff_angle_abs < 200.0){
            float vel = diff_angle * 500000.0;
            unsigned long dt = diff_time * 3;
            vel = vel * 10.0;
            vel = vel / dt;
            vel = vel / _m_param->parameter(PARAM_MOTOR_RATIO);
            _motor_velocity_smooth_count++;
            if(_motor_velocity_smooth_count == 3){
                _motor_velocity_smooth_count = 0;
            }
            _motor_velocity_smooth[_motor_velocity_smooth_count] = vel;

            float vel_smooth = _motor_velocity_smooth[0] + _motor_velocity_smooth[1] + _motor_velocity_smooth[2];
            vel_smooth = vel_smooth / 3.0;
            _m_param->parameter(PARAM_MOTOR_VELOCITY, round(vel_smooth));

            // Serial.print("v: ");
            // Serial.println(round(vel_smooth));
        }
        _angle_last_read_motor_hall = motor_angle;
        _time_last_read_motor_hall = micros();
    }
}
void Musicon_v2::_readSetterHall(){
    int err = _multiplexer(0);
    if(err > 0) return;
    if(_m_param->parameter(PARAM_I2C_STATUS) == 1 && _ams5600.detectMagnet() == 1 ){
        float kat_silnika = _convertRawAngleToDegrees(_ams5600.getRawAngle());
        int kat = (int)kat_silnika;
        _m_param->parameter(PARAM_HALL_SETTER_DEG, kat);
        _setter_hall_init_ok = true;
    }
}
void Musicon_v2::_menu_sw_control(){
    _m_param->parameter(PARAM_MENU_SW_LIVE_ADC, analogRead(MENU_SW_PIN));
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_MINUS)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_MINUS)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 2);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _menu_sw_press_time(_time_start_menu_sw_ms);
        }
    }
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_PLUS)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_PLUS)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 1);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _menu_sw_press_time(_time_start_menu_sw_ms);
        }
    }    
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_MINUSPLUS)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_MINUSPLUS)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 3);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _menu_sw_press_time(_time_start_menu_sw_ms);
        }
    }    
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_OK)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_OK)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 4);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _menu_sw_press_time(_time_start_menu_sw_ms);
        }
    }    
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > 1000){
        _m_param->parameter(PARAM_MENU_SW_LIVE, 0);
        _m_param->parameter(PARAM_MENU_SW_TIME, 0);
        _time_start_menu_sw_ms = 0;
    }
    // Serial.print("PARAM_MENU_SW_LIVE: ");
    // Serial.println(_m_param->parameter(PARAM_MENU_SW_LIVE));
    // Serial.print("PARAM_MENU_SW_TIME: ");
    // Serial.println(_m_param->parameter(PARAM_MENU_SW_TIME));
}
void Musicon_v2::_menu_sw_press_time(unsigned long start_time){
    unsigned long diff = (millis() - start_time);
    if(diff < 32000)
        _m_param->parameter(PARAM_MENU_SW_TIME, (int)diff);
}
int Musicon_v2::_multiplexer (uint8_t ch) {
    if (ch > 1) return 1;
    uint8_t error = 0;
    uint8_t chann = 0;
    int mplx_type = this->_m_param->parameter(PARAM_MPLX_TYPE);

    if(mplx_type == 43 || mplx_type == 42){
        if(mplx_type == 43) chann = 1 << ch;
        if(mplx_type == 42) chann = 4 + ch;

        Wire.beginTransmission(MPLXADR);
        Wire.write(chann);
        Wire.endTransmission();
        // Wire.beginTransmission(0x36);
        // error = Wire.endTransmission();
        // if(error == 0){
        //     _m_param->parameter(PARAM_I2C_STATUS, 1);     
        //     return 0; 
        // }else{
        //     _m_param->parameter(PARAM_I2C_STATUS, 0);
        //     _m_param->parameter(PARAM_MPLX_TYPE, 0);
        //     return 1;
        // }
    }else{
        error = 0;
        chann = 1 << ch;
        Wire.beginTransmission(MPLXADR);
        Wire.write(chann);
        Wire.endTransmission();

        Wire.beginTransmission(0x36);
        error = Wire.endTransmission();

        if(error == 0){
            _m_param->parameter(PARAM_I2C_STATUS, 1);
            _m_param->parameter(PARAM_MPLX_TYPE, 43);
            return 0;
        }else{
            error = 0;
            chann = 4 + ch;
            Wire.beginTransmission(MPLXADR);
            Wire.write(chann);
            Wire.endTransmission();

            Wire.beginTransmission(0x36);
            error = Wire.endTransmission();
            if(error == 0){
                _m_param->parameter(PARAM_I2C_STATUS, 1);
                _m_param->parameter(PARAM_MPLX_TYPE, 42);
                return 0;
            }else{
                _m_param->parameter(PARAM_I2C_STATUS, 0);
                _m_param->parameter(PARAM_MPLX_TYPE, 0);  
                return 2;             
            }
        }
    }
    // //Wire.write(1 << ch);
    // int adr = 4 + ch;
    // //   if (ch == 0) Wire.write(0b00000100);
    // //   if (ch == 1) Wire.write(0b00000101);
    // Wire.beginTransmission(MPLXADR);
    // Wire.write(1 << ch);
    // Wire.endTransmission();
    
    // Wire.beginTransmission(0x36);
    // uint8_t error = Wire.endTransmission();

    // if (error != 0){
    //     Wire.beginTransmission(MPLXADR);
    //     Wire.write(adr);
    //     Wire.endTransmission();
    //     Wire.beginTransmission(0x36);
    //     error = Wire.endTransmission();
    // }   

    // if (error == 0){
    //     _m_param->parameter(PARAM_I2C_STATUS, 1);
    // }else{
    //     _m_param->parameter(PARAM_I2C_STATUS, 0);
    // }
}

float Musicon_v2::_convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087912 of a degree */
  float retVal = newAngle * 0.87912;
  return retVal;
}
void Musicon_v2::_clickSetterLimitSwitch(){
    int lim_sw_status = _limitSwitchUpd();
    int act_deg_position = _m_param->parameter(PARAM_HALL_SETTER_DEG);
    _m_param->parameter(PARAM_SETTER_LIM_SW_STAT, lim_sw_status);
    if(lim_sw_status == 1){
        _readSetterHall();
        act_deg_position = _m_param->parameter(PARAM_HALL_SETTER_DEG);
        if(_m_param->parameter(PARAM_SETTER_MIN_VALUE) == -1 &&
           _m_param->parameter(PARAM_SETTER_MAX_VALUE) == -1){
               _m_param->parameter(PARAM_SETTER_MIN_VALUE, act_deg_position, true);
               _m_param->parameter(PARAM_SETTER_MAX_VALUE, act_deg_position, true);
        }else{
            int diff = abs(_m_param->parameter(PARAM_SETTER_MIN_VALUE) - _m_param->parameter(PARAM_SETTER_MAX_VALUE));
            if(diff == 0){
                int diff_next = abs(_m_param->parameter(PARAM_SETTER_MIN_VALUE) - act_deg_position);
                if(diff_next < 1800){
                    if(_m_param->parameter(PARAM_SETTER_MIN_VALUE) < act_deg_position){
                        _m_param->parameter(PARAM_SETTER_MAX_VALUE, act_deg_position, true);
                    }else{
                        _m_param->parameter(PARAM_SETTER_MIN_VALUE, act_deg_position, true);
                    }
                }else{
                    if(_m_param->parameter(PARAM_SETTER_MIN_VALUE) > act_deg_position){
                        _m_param->parameter(PARAM_SETTER_MAX_VALUE, act_deg_position, true);
                    }else{
                        _m_param->parameter(PARAM_SETTER_MIN_VALUE, act_deg_position, true);
                    } 
                }
            }
        }
    }
    if(lim_sw_status == 2){
        int diff_min = abs(_m_param->parameter(PARAM_SETTER_MIN_VALUE) - act_deg_position);
        int diff_max = abs(_m_param->parameter(PARAM_SETTER_MAX_VALUE) - act_deg_position);
            // Serial.print("min: ");
            // Serial.print(diff_min);
            // Serial.print(" , ");
            // Serial.print(diff_max);
            // Serial.print(" , ");
            // Serial.println(act_deg_position);
        if(diff_min > 150 && diff_max > 150){
            _m_param->parameter(PARAM_SETTER_MIN_VALUE, -1);
            _m_param->parameter(PARAM_SETTER_MAX_VALUE, -1);
        }
    }
}
void Musicon_v2::_tactCalculate(){
    int fromLow = _m_param->parameter(PARAM_SETTER_MIN_VALUE);
    int fromHigh = _m_param->parameter(PARAM_SETTER_MAX_VALUE);
    int diff = abs(fromLow - fromHigh);
    int act_deg_position = _m_param->parameter(PARAM_HALL_SETTER_DEG);
    int tact = 0;
    if(diff < 300){
        _m_param->parameter(PARAM_INIT_OK_SETTER, 0);
        return;
    }else{
        _m_param->parameter(PARAM_INIT_OK_SETTER, 1);
    }
    if(diff > 1800){
        fromHigh = _m_param->parameter(PARAM_SETTER_MAX_VALUE) + 3600;
        if(act_deg_position < 1800){
            act_deg_position += 3600;
        }
    }
    fromLow += 20;
    fromHigh -= 20;
    tact = map(act_deg_position, fromLow, fromHigh, _m_param->parameter(PARAM_TACT_MIN_VALUE), _m_param->parameter(PARAM_TACT_MAX_VALUE));
    tact = constrain(tact, _m_param->parameter(PARAM_TACT_MIN_VALUE), _m_param->parameter(PARAM_TACT_MAX_VALUE));
    _m_param->parameter(PARAM_SETTER_TACT, tact);
}
int Musicon_v2::_limitSwitchUpd(){
    //return:  0=button is relased, 2=button is pressed, -1=falling edge, 1=rising edge
        if(digitalRead(LIMIT_SWITCH_PIN) == HIGH && _lim_sw_mem) return 0;
        if(digitalRead(LIMIT_SWITCH_PIN) == HIGH && !_lim_sw_mem){  _lim_sw_mem = true; return -1; } 
        if(digitalRead(LIMIT_SWITCH_PIN) == LOW && _lim_sw_mem){  _lim_sw_mem = false; return 1; } 
        if(digitalRead(LIMIT_SWITCH_PIN) == LOW && !_lim_sw_mem) return 2;
}