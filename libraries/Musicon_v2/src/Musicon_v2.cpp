#include <Musicon_v2.h>

void Musicon_v2::init(){
    analogReference( INTERNAL );
    _m_param = new M_param();
    Serial.begin(57600);
    Wire.begin();
    _m_power->init();
    _m_lcd = new M_lcd(_m_param);
    // Serial.print("Adres: ");
    // Serial.println(M_lcd.getAdress());
    // Serial.println(0x27);
}
 
void Musicon_v2::main(){
    _cycleTimeCheck();
    _m_lcd->refresh();
    if((millis() % 80) == 0) _menu_sw_control();
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
            _m_param->parameter(PARAM_MENU_SW_TIME, ((millis() - _time_start_menu_sw_ms)/1000));
        }
    }
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_PLUS)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_PLUS)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 1);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _m_param->parameter(PARAM_MENU_SW_TIME, ((millis() - _time_start_menu_sw_ms)/1000));
        }
    }    
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_MINUSPLUS)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_MINUSPLUS)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 3);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _m_param->parameter(PARAM_MENU_SW_TIME, ((millis() - _time_start_menu_sw_ms)/1000));
        }
    }    
    if(_m_param->parameter(PARAM_MENU_SW_LIVE_ADC) > (_m_param->parameter(PARAM_MENU_SW_OK)-_m_param->parameter(PARAM_MENU_SW_TOLERANCE)) 
        && _m_param->parameter(PARAM_MENU_SW_LIVE_ADC) < (_m_param->parameter(PARAM_MENU_SW_OK)+_m_param->parameter(PARAM_MENU_SW_TOLERANCE)))
    {
        _m_param->parameter(PARAM_MENU_SW_LIVE, 4);
        if(_time_start_menu_sw_ms == 0){
            _time_start_menu_sw_ms = millis();
        }else{
            _m_param->parameter(PARAM_MENU_SW_TIME, ((millis() - _time_start_menu_sw_ms)/1000));
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