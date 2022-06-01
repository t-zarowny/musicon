#include <M_power.h>

M_power::M_power(M_data* param, M_lcd* lcd){
    _param = param;
    _lcd = lcd;
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, HIGH);
    pinMode(SW_PIN, INPUT);
    pinMode(BAT_CHARGE_PIN,OUTPUT);
    _param->parameter(PARAM_POWER_STATUS, 3);
}
void M_power::refresh(){
    if((millis() % 200) == 0) {
        _check_batt_v();
        _check_batt_current();
        _batStatusCalc();
    }
    _powerManager();
    if((_mem_time_led + 1000) < millis()){
      _ledPowerButton();
      _powerSupplyStat();
      _battery_charging();
      _mem_time_led = millis();
    }
}
void M_power::_check_batt_v(){
    int v_out = analogRead(VBATT_PIN) * (1.1/1024) * 206;
    _bat_V_smooth[_bat_V_smooth_nr] = v_out;
    _bat_V_smooth_nr++;
    if(_bat_V_smooth_nr == 10)
      _bat_V_smooth_nr = 0;
    v_out = 0;
    for(int i=0; i < 10; i++){
      v_out += _bat_V_smooth[i];
    }
    v_out = v_out / 10;
    _param->parameter(PARAM_POWER_BAT_V, v_out);
    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 1){
      this->_motor_test = true;
    }
    if(_param->parameter(PARAM_POWER_BAT_V) - _param->parameter(PARAM_POWER_BAT_V_CALC) > 20){
      this->_motor_test = false;
      _param->parameter(PARAM_POWER_BAT_V_CALC, _param->parameter(PARAM_POWER_BAT_V)-8);
    }
    if(_param->parameter(PARAM_POWER_BAT_CHARGE) == 1){
      this->_motor_test = false;
      _param->parameter(PARAM_POWER_BAT_V_CALC, _param->parameter(PARAM_POWER_BAT_V)-8);
    }else{
      if(!this->_motor_test){
        int v_min = min(_param->parameter(PARAM_POWER_BAT_V_CALC),_param->parameter(PARAM_POWER_BAT_V)-8);
        _param->parameter(PARAM_POWER_BAT_V_CALC, v_min);
      }else{
        int v_min = min(_param->parameter(PARAM_POWER_BAT_V_CALC),_param->parameter(PARAM_POWER_BAT_V));
        _param->parameter(PARAM_POWER_BAT_V_CALC, v_min);
      }
    }
    // long watt = (v_out * _param->parameter(PARAM_POWER_BAT_MA)) / -10000;
    // int v_calc = v_out + watt;
    // _param->parameter(PARAM_POWER_BAT_V_CALC, v_calc);
}
void M_power::_check_batt_current(){
    int a_out = analogRead(CSENS_PIN)-510;
    a_out = a_out * 3.2552 * 2;
    _bat_A_smooth[_bat_A_smooth_nr] = a_out;
    _bat_A_smooth_nr++;
    if(_bat_A_smooth_nr == 10)
      _bat_A_smooth_nr = 0;
    a_out = 0;
    for(int i=0; i < 10; i++){
      a_out += _bat_A_smooth[i];
    }
    a_out = a_out / 10;
    _param->parameter(PARAM_POWER_BAT_MA, a_out);

    if((_param->parameter(PARAM_POWER_SUPPLY_OK) == 1) && (_param->parameter(PARAM_POWER_BAT_MA) > 200)){
      _param->parameter(PARAM_POWER_BAT_CHARGE, 1);
    }
    if((_param->parameter(PARAM_POWER_SUPPLY_OK) == 0) || (_param->parameter(PARAM_POWER_BAT_MA) < 100)){
      _param->parameter(PARAM_POWER_BAT_CHARGE, 0);
    }
}
void M_power::_powerManager(){
  // ustawienie zmiennej przycisku zasilania tylko raz, niezależnie od długości wciśnięcia
  if(digitalRead(SW_PIN) == HIGH && this->_sw_switch_mem == false){
    this->_sw_switch = true;
  }else{
    this->_sw_switch = false;
  }
  this->_sw_switch_mem = digitalRead(SW_PIN);  
  
  //jeżeli włączony i wciśnięto przycisk - przejdź do wyłączania
  if(_param->parameter(PARAM_POWER_STATUS) == 3 && this->_sw_switch){      
        _param->parameter(PARAM_POWER_STATUS, 4);
        _lcd->status(3);
  }

    if(_param->parameter(PARAM_POWER_STATUS) == 0 && _param->parameter(PARAM_POWER_BAT_CHARGE) == 1){
      _param->parameter(PARAM_POWER_STATUS, 1);
      _lcd->status(101);
    }
    if(_param->parameter(PARAM_POWER_STATUS) == 1 && _param->parameter(PARAM_POWER_BAT_CHARGE) == 0){
      _param->parameter(PARAM_POWER_STATUS, 4);
      _lcd->status(3);
    }
    if((_param->parameter(PARAM_POWER_STATUS) == 0 || _param->parameter(PARAM_POWER_STATUS) == 1) && 
        (this->_sw_switch || abs(_param->parameter(PARAM_MOTOR_VELOCITY)) > 30)){
      _param->parameter(PARAM_POWER_STATUS, 3);
      digitalWrite(ENABLE_PIN, HIGH);
      _lcd->status(0);
      analogWrite(LED_SW_PIN, 255);
      _param->parameter(PARAM_INACTION_TIME,0);
    } 
    if(_param->parameter(PARAM_POWER_STATUS) == 3 && (_param->parameter(PARAM_INACTION_TIME) > _param->parameter(PARAM_INACTION_TIME_OFF)) && _param->parameter(PARAM_INACTION_TIME_OFF) > 0){
      _param->parameter(PARAM_POWER_STATUS, 4);
      _lcd->status(3);
    }
    if(_param->parameter(PARAM_POWER_STATUS) == 4 && _lcd->status() == 100){
        _param->parameter(PARAM_POWER_STATUS, 0);
        analogWrite(LED_SW_PIN, 0);
        digitalWrite(ENABLE_PIN, LOW);
    }
}
void M_power::_ledPowerButton(){
  if(_param->parameter(PARAM_POWER_STATUS) == 3){
    led_value = 255;
  }
  if(_param->parameter(PARAM_POWER_STATUS) == 0){
    led_value = 0;
  }
  if(_param->parameter(PARAM_POWER_STATUS) == 1){
    if(led_value == 0){
      led_value = 255;
    }else{
      led_value = 0;
    }
  }
  analogWrite(LED_SW_PIN, led_value);
}
void M_power::_powerSupplyStat(){
  if(_param->parameter(PARAM_POWER_BAT_MA) > -40){
    _param->parameter(PARAM_POWER_SUPPLY_OK, 1);
  }else{
    _param->parameter(PARAM_POWER_SUPPLY_OK, 0);
  }
}
void M_power::_batStatusCalc(){
  if(_param->parameter(PARAM_POWER_BAT_V) < 5){
    _param->parameter(PARAM_POWER_BAT_STAT, (-1));
  }else{
    int v_bat = _param->parameter(PARAM_POWER_BAT_V_CALC);
    if(v_bat < 127) v_bat = 127;
    if(v_bat > 149) v_bat = 149;
    int batt_percent = (int)map(v_bat,127,149,0,100);
    _param->parameter(PARAM_POWER_BAT_STAT, batt_percent);
  }
}
void M_power::_battery_charging(){
  if((_param->parameter(PARAM_POWER_BAT_STAT) < 0) || _param->parameter(PARAM_POWER_SUPPLY_OK) == 0){
    _bat_charge_pwm = 64;
  }
  if(_param->parameter(PARAM_POWER_BAT_MA) < _param->parameter(PARAM_POWER_BAT_A_MAX)){
    if(_bat_charge_pwm < 255 && _param->parameter(PARAM_POWER_SUPPLY_OK) == 1)
      _bat_charge_pwm++;
  }
  if(_param->parameter(PARAM_POWER_BAT_MA) > _param->parameter(PARAM_POWER_BAT_A_MAX) + 300){
    if(_bat_charge_pwm > 0 && _param->parameter(PARAM_POWER_SUPPLY_OK) == 1)
      _bat_charge_pwm--;
  }
  _param->parameter(PARAM_POWER_BAT_PWM, _bat_charge_pwm);
  analogWrite(BAT_CHARGE_PIN, _bat_charge_pwm);  
}