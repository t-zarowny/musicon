#include <M_power.h>

M_power::M_power(M_param* param){
    _param = param;
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, HIGH);
    analogWrite(LED_SW_PIN, 255);
}
void M_power::refresh(){
    if((millis() % 200) == 0) {
        _check_batt_v();
        _check_batt_current();
    }
}
void M_power::_check_batt_v(){
    int v_out = analogRead(VBATT_PIN) * (1.1/1024) * 206;
    _param->parameter(PARAM_POWER_BAT_V, v_out);
    long watt = (v_out * _param->parameter(PARAM_POWER_BAT_MA)) / -10000;
    int v_calc = v_out + watt;
    _param->parameter(PARAM_POWER_BAT_V_CALC, v_calc);
}
void M_power::_check_batt_current(){
    int a_out = analogRead(CSENS_PIN)-510;
    a_out = a_out * 3.2552 * 2;
    _param->parameter(PARAM_POWER_BAT_MA, a_out);
}