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
void M_lcd::_menu_control(){

}
void M_lcd::_intro(){
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
        if((millis() % 100) == 0) _intro();
        break;
    case 1:
        if((millis() % 500) == 0) _default_screen();
        if((_param->parameter(PARAM_MENU_SW_LIVE) == 1 || 
            _param->parameter(PARAM_MENU_SW_LIVE) == 2 ||
            _param->parameter(PARAM_MENU_SW_LIVE) == 4) && 
            !_button_pressed ){
                lcd->clear();
                _status = 2;
                _button_pressed = true;
        }
        break;
     case 2:
        if((millis() % 100) == 0) _screen_menu();
        if(_param->parameter(PARAM_MENU_SW_LIVE) == 3 && 
            !_button_pressed ){
                lcd->clear();
                _status = 1;
                _menu_display_nr = 0;
                _button_pressed = true;
        }
        if(_param->parameter(PARAM_MENU_SW_LIVE) == 1 && 
            !_button_pressed ){
                int next_nr = _menu[_menu_display_nr].up;
                if(next_nr >= 0)
                    _menu_display_nr = next_nr;
                _button_pressed = true;
        }       
        if(_param->parameter(PARAM_MENU_SW_LIVE) == 2 && 
            !_button_pressed ){
                int next_nr = _menu[_menu_display_nr].down;
                if(next_nr >= 0)
                    _menu_display_nr = next_nr;
                _button_pressed = true;
        }       
        if(_param->parameter(PARAM_MENU_SW_LIVE) == 4 && 
            !_button_pressed ){
                int next_nr = _menu[_menu_display_nr].enter;
                if(next_nr >= 0)
                    _menu_display_nr = next_nr;
                _button_pressed = true;
        }       
        break;   
    default:
        break;
    }
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 0)
        _button_pressed = false;
}
void M_lcd::_default_screen(){
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
void M_lcd::_screen_menu(){
    int up_line = _menu_display_nr;
    int down_line = _menu[_menu_display_nr].down;
    lcd->setCursor(0,0);
    lcd->print(">");
    lcd->print(_menu[up_line].text);
    lcd->setCursor(0,1);
    if(down_line < 0){
        lcd->print("                ");
    }else{
        lcd->print(" ");
        lcd->print(_menu[down_line].text);
    }
}
