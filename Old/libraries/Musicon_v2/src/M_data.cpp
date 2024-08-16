#include <M_data.h>

M_data::M_data(){
  _init();
  // restore();
}
void M_data::_init(){
  for(int i = 0; i < 200; i++){ 
    _readParamToEEPROM(i); 
    if(i==199) init_ok = true;
  }
  if(_parameters[PARAM_RESTORE] != 1001) 
    restore();
  _parameters[PARAM_TACT_MAX_VALUE] = (_parameters[PARAM_MOTOR_TMC_MAX_VEL]*32)/10;
  _parameters[PARAM_TACT_MIN_VALUE] = (_parameters[PARAM_MOTOR_TMC_MIN_VEL]*32)/10;
}

// Odczytuje parametr.
// param_nr => numer parametru
int M_data::parameter(int param_nr){
  return _parameters[param_nr];
}

// Modyfikuje parametr.
// param_nr => numer parametru
// change_values => nowa wartość parametru
// save_to_eeprom => czy zapisać do pamięci
int M_data::parameter(int param_nr, int change_values, bool save_to_eeprom = false){
  _parameters[param_nr] = change_values;
  if(save_to_eeprom){
    _writeParamToEEPROM(param_nr, change_values);
  }
  if(param_nr == PARAM_MOTOR_TMC_MAX_VEL || param_nr == PARAM_MOTOR_TMC_MIN_VEL){
    _parameters[PARAM_TACT_MAX_VALUE] = (_parameters[PARAM_MOTOR_TMC_MAX_VEL]*32)/10;
    _parameters[PARAM_TACT_MIN_VALUE] = (_parameters[PARAM_MOTOR_TMC_MIN_VEL]*32)/10; 
  }
  return _parameters[param_nr];
}

// Zapis parametru do pamięci EEPROM. 
// param_nr => numer parametru
// val => nowa wartość parametru
void M_data::_writeParamToEEPROM(int param_nr, int val)
{
  int addressIndex = param_nr*2;
  EEPROM.write(addressIndex, val >> 8);
  EEPROM.write(addressIndex + 1, val & 0xFF);
}

// Odczyt parametru z pamięci EEPROM.
// start_address => adres pierwszego bajtu
// param => wskaźnik na tablicę z parametrami
// param_nr => numer parametru
void M_data::_readParamToEEPROM(int param_nr)
{
  int addressIndex = param_nr*2;
  _parameters[param_nr] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
}

// Przywraca wartości domyślne wszystkich parametrów.
void M_data::restore(){
  for(int i=0;i<200;i++){
    this->parameter(i, 0, true);
  }
  this->parameter(PARAM_RESTORE,                1001, true);
  this->parameter(PARAM_POWER_STATUS,           3,    true);
  this->parameter(PARAM_MENU_SW_MINUS,          510,  true);
  this->parameter(PARAM_MENU_SW_PLUS,           320,  true);
  this->parameter(PARAM_MENU_SW_MINUSPLUS,      245,  true);
  this->parameter(PARAM_MENU_SW_OK,             170,  true);
  this->parameter(PARAM_MENU_SW_TOLERANCE,      30,   true);
  this->parameter(PARAM_INIT_OK_SETTER,         0,    true);
  this->parameter(PARAM_SETTER_MIN_VALUE,       -1,   true);
  this->parameter(PARAM_SETTER_MAX_VALUE,       -1,   true);
  this->parameter(PARAM_MOTOR_TMC_VELOCITY,     1770, true);
  this->parameter(PARAM_MOTOR_TMC_MIN_VEL,      10,   true);
  this->parameter(PARAM_MOTOR_TMC_MAX_VEL,      45,   true);
  this->parameter(PARAM_MOTOR_RATIO,            60,   true);
  this->parameter(PARAM_MOTOR_BRAKE_VEL,        60,   true);
  this->parameter(PARAM_MOTOR_BRAKE_TIME,       100,  true);
  this->parameter(PARAM_POWER_BAT_MIN,          127,  true);
  this->parameter(PARAM_POWER_BAT_A_MAX,        600,  true);
  this->parameter(PARAM_INACTION_TIME_OFF,      600,  true);
  this->parameter(PARAM_NOT_TURN_OFF_WHILE_AC,  0,    true);
}