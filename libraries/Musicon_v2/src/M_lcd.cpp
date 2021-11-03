#include <M_lcd.h>

M_lcd::M_lcd(M_param* param){
    _param = param;
    //_param->parameter(PARAM_POWER_STATUS,1);
    _checkAdress(LCD_ADRESS_1);
    _checkAdress(LCD_ADRESS_2);
    if(_adress != 0){
        lcd = new LiquidCrystal_I2C(_adress, 16, 2);
        lcd->begin(); 
        lcd->clear();
    }
}
void M_lcd::_checkAdress(uint8_t adr){
    Wire.beginTransmission(adr);
    if(Wire.endTransmission() == 0) 
        _adress = adr;
}
void M_lcd::intro(){
    switch (_intro_step)
    {
    case 15:
        lcd->setCursor(1,0); lcd->print("-"); break;
    case 14:
        lcd->setCursor(1,0); lcd->print("M"); break;
    case 13:
        lcd->setCursor(3,0); lcd->print("-"); break;
    case 12:
        lcd->setCursor(3,0); lcd->print("U"); break;
    case 11:
        lcd->setCursor(5,0); lcd->print("-"); break;
    case 10:
        lcd->setCursor(5,0); lcd->print("S"); break;
    case 9:
        lcd->setCursor(7,0); lcd->print("-"); break;
    case 8:
        lcd->setCursor(7,0); lcd->print("I"); break;
    case 7:
        lcd->setCursor(9,0); lcd->print("-"); break;
    case 6:
        lcd->setCursor(9,0); lcd->print("C"); break;
    case 5:
        lcd->setCursor(11,0); lcd->print("-"); break;
    case 4:
        lcd->setCursor(11,0); lcd->print("O"); break;
    case 3:
        lcd->setCursor(13,0); lcd->print("-"); break;
    case 2:
        lcd->setCursor(13,0); lcd->print("N"); break;
    default:
        break;
    }
    _intro_step--;
    if(_intro_step == 0) _status = 1;
}
void M_lcd::refresh(){
    switch (_status)
    {
    case 0:
        if((millis() % 100) == 0) intro();
        break;
    case 1:
        if((millis() % 500) == 0) default_screen();
        break;
    
    default:
        break;
    }
}
void M_lcd::default_screen(){
    lcd->setCursor(0,0);
    lcd->print(" M U S I C O N  ");
    //lcd->setCursor(0,1);
    //lcd->print("Tempo: ");
    lcd->setCursor(0,1);
    lcd->print(_param->parameter(PARAM_CYCLE_MIN_TIME));
    lcd->print("-");
    lcd->print(_param->parameter(PARAM_CYCLE_MAX_TIME));
    lcd->print(" ");
    lcd->setCursor(11,1);
    lcd->print(_param->parameter(PARAM_MENU_SW_LIVE_ADC));
    lcd->print("  ");
}