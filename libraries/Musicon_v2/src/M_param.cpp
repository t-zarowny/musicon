#include <M_param.h>

M_param::M_param(){
  _init();
  //restore();
}
void M_param::_init(){
  for(int i = 0; i < 200; i++){ 
    _readParamToEEPROM(i); 
  }
  if(_parameters[PARAM_RESTORE] != 1001) 
    restore();
}

// Odczytuje parametr.
// param_nr => numer parametru
int M_param::parameter(int param_nr){
  return _parameters[param_nr];
}

// Modyfikuje parametr.
// param_nr => numer parametru
// change_values => nowa wartość parametru
// save_to_eeprom => czy zapisać do pamięci
int M_param::parameter(int param_nr, int change_values, bool save_to_eeprom = false){
  _parameters[param_nr] = change_values;
  if(save_to_eeprom){
    _writeParamToEEPROM(param_nr, change_values);
  }
  return _parameters[param_nr];
}

// Zapis parametru do pamięci EEPROM. 
// param_nr => numer parametru
// val => nowa wartość parametru
void M_param::_writeParamToEEPROM(int param_nr, int val)
{
  int addressIndex = param_nr*2;
  EEPROM.write(addressIndex, val >> 8);
  EEPROM.write(addressIndex + 1, val & 0xFF);
}

// Odczyt parametru z pamięci EEPROM.
// start_address => adres pierwszego bajtu
// param => wskaźnik na tablicę z parametrami
// param_nr => numer parametru
void M_param::_readParamToEEPROM(int param_nr)
{
  int addressIndex = param_nr*2;
  _parameters[param_nr] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
}

// Przywraca wartości domyślne wszystkich parametrów.
void M_param::restore(){
  for(int i=0;i<200;i++){
    parameter(i, 0, true);
  }
  parameter(PARAM_RESTORE,            1001, true);
  parameter(PARAM_POWER_STATUS,       3,    true);
  parameter(PARAM_MENU_SW_MINUS,      510,  true);
  parameter(PARAM_MENU_SW_PLUS,       320,  true);
  parameter(PARAM_MENU_SW_MINUSPLUS,  245,  true);
  parameter(PARAM_MENU_SW_OK,         170,  true);
  parameter(PARAM_MENU_SW_TOLERANCE,  30,   true);
  parameter(PARAM_INIT_OK_SETTER,     0,    true);
  parameter(PARAM_SETTER_MIN_VALUE,     -1,    true);
  parameter(PARAM_SETTER_MAX_VALUE,     -1,    true);
}