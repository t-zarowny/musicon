#include <M_param.h>

void M_param::init(){
  _init_group(GROUP_POWER);
}
void M_param::_init_group(int param_group){
    switch (param_group)
    {
        case GROUP_POWER:
            for(int i = 0; i < PARAM_POWER_QUANTITY; i++){ 
              _readParamToEEPROM(PARAM_POWER_EPPROM_START, _param_power_user, i); 
            }
            if(_param_power_user[0] == 0) 
              restore(GROUP_POWER);
            _paramCopy(_param_power_user,_param_power,PARAM_POWER_QUANTITY);
            break;
        
        default:
            break;
    }
}
void M_param::power(int param_nr, int change_values, bool save_to_eeprom = false){
  _changeParam(_param_power,_param_power_user, PARAM_POWER_EPPROM_START, param_nr, change_values, save_to_eeprom);
}
int M_param::power(int param_nr){
  return _param_power[param_nr];
}
void M_param::_changeParam(int* param_arr, int* param_arr_user, int eeprom_start, int param_nr, int change_values, bool save_to_eeprom){
  param_arr[param_nr] = change_values;
  if(save_to_eeprom){
    param_arr_user[param_nr] = change_values;
    _writeParamToEEPROM(eeprom_start, param_arr_user, param_nr);
  }
}
void M_param::restore(int param_group){
  switch (param_group)
  {
  case GROUP_POWER:
    power(PARAM_POWER_RESTORE, 1, true);
    break;
  
  default:
    break;
  }
}
void M_param::_writeParamToEEPROM(int start_address, int param[], int param_nr)
{
  int addressIndex = start_address + (param_nr*2);
  EEPROM.write(addressIndex, param[param_nr] >> 8);
  EEPROM.write(addressIndex + 1, param[param_nr] & 0xFF);
}
void M_param::_readParamToEEPROM(int start_address, int param[], int param_nr)
{
  int addressIndex = start_address + (param_nr*2);
  param[param_nr] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
}
void M_param::_paramCopy(int* src, int* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}