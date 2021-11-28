#include <Musicon_v2.h>

void Musicon_v2::init(){
    analogReference( INTERNAL );
    pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
    _m_param = new M_param();
    Serial.begin(57600);
    Wire.begin();
    _m_lcd = new M_lcd(_m_param);
    _m_power = new M_power(_m_param);
    _ams5600 = new AMS_5600();
    // Serial.print("Adres: ");
    // Serial.println(M_lcd.getAdress());
    // Serial.println(0x27);
}
 
void Musicon_v2::main(){
    _cycleTimeCheck();
    _m_lcd->refresh();
    _m_power->refresh();
    if((millis() % 10) == 0) _readMagnetHall();
    if((millis() % 40) == 0) _menu_sw_control();
    _clickSetterLimitSwitch();
    _tactCalculate();
}
void Musicon_v2::_cycleTimeCheck(){
        _count_cycle++;
        _time_act_us = micros();
        int diff = _time_act_us - _time_cycle_last_us;
        _time_cycle_last_us = _time_act_us;
        if(diff > _time_cycle_max_us){
            _time_cycle_max_us = diff;
        }
        if(diff < _time_cycle_min_us){
            _time_cycle_min_us = diff;
        }
        if(_count_cycle == 1000){
            _m_param->parameter(PARAM_CYCLE_MAX_TIME, _time_cycle_max_us);
            _m_param->parameter(PARAM_CYCLE_MIN_TIME, _time_cycle_min_us);
            _count_cycle = 0;
            _time_cycle_max_us = 0;
            _time_cycle_min_us = 32000;
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
void Musicon_v2::_multiplexer (uint8_t ch) {
  if (ch > 1) return;
  Wire.beginTransmission(MPLXADR);
  //Wire.write(1 << ch);
  if (ch == 0) Wire.write(0b00000100);
  if (ch == 1) Wire.write(0b00000101);
  Wire.endTransmission();
}
void Musicon_v2::_readMagnetHall(){
    switch (_nextHallRead)
    {
    case 0:
        _multiplexer(0);
        _nextHallRead = 10;
        break;
    case 1:
        _multiplexer(1);
        _nextHallRead = 11;
        break;
    case 10:
        if(_ams5600->detectMagnet() == 1 ){
            float kat_silnika = _convertRawAngleToDegrees(_ams5600->getRawAngle());
            int kat = (int)kat_silnika;
            _m_param->parameter(PARAM_HALL_SETTER_DEG, kat);
        }
        _nextHallRead = 1;
        break;  
    case 11:
        if(_ams5600->detectMagnet() == 1 ){
            float kat_silnika = _convertRawAngleToDegrees(_ams5600->getRawAngle());
            int kat = (int)kat_silnika;
            _m_param->parameter(PARAM_HALL_MOTOR_DEG, kat);
        }
        _nextHallRead = 0;
        break;  
    default:
        break;
    }
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
        _multiplexer(0);
        _nextHallRead = 10;
        _readMagnetHall();
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
    tact = map(act_deg_position, fromLow, fromHigh, 32, 144);
    tact = constrain(tact, 32, 144);
    _m_param->parameter(PARAM_SETTER_TACT, tact);
}
int Musicon_v2::_limitSwitchUpd(){
    //return:  0=button is relased, 2=button is pressed, -1=falling edge, 1=rising edge
        if(digitalRead(LIMIT_SWITCH_PIN) == HIGH && _lim_sw_mem) return 0;
        if(digitalRead(LIMIT_SWITCH_PIN) == HIGH && !_lim_sw_mem){  _lim_sw_mem = true; return -1; } 
        if(digitalRead(LIMIT_SWITCH_PIN) == LOW && _lim_sw_mem){  _lim_sw_mem = false; return 1; } 
        if(digitalRead(LIMIT_SWITCH_PIN) == LOW && !_lim_sw_mem) return 2;
}