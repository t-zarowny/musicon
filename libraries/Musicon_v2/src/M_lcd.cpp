#include <M_lcd.h>

M_lcd::M_lcd(M_data* param, LiquidCrystal_I2C* l){
    _param = param;
    this->lcd = l;
}
M_lcd::M_lcd(){}

void M_lcd::_menu_control(){

}
void M_lcd::_intro(){
    lcd->backlight();
    switch (_intro_step)
    {
    case 20:
        lcd->setCursor(1,0); lcd->print(F("-")); break;
    case 19:
        lcd->setCursor(1,0); lcd->print(F("M")); break;
    case 18:
        lcd->setCursor(3,0); lcd->print(F("-")); break;
    case 17:
        lcd->setCursor(3,0); lcd->print(F("U")); break;
    case 16:
        lcd->setCursor(5,0); lcd->print(F("-")); break;
    case 15:
        lcd->setCursor(5,0); lcd->print(F("S")); break;
    case 14:
        lcd->setCursor(7,0); lcd->print(F("-")); break;
    case 13:
        lcd->setCursor(7,0); lcd->print(F("I")); break;
    case 12:
        lcd->setCursor(9,0); lcd->print(F("-")); break;
    case 11:
        lcd->setCursor(9,0); lcd->print(F("C")); break;
    case 10:
        lcd->setCursor(11,0); lcd->print(F("-")); break;
    case 9:
        lcd->setCursor(11,0); lcd->print(F("O")); break;
    case 8:
        lcd->setCursor(13,0); lcd->print(F("-")); break;
    case 7:
        lcd->setCursor(13,0); lcd->print(F("N")); break;
    default:
        break;
    }
    _intro_step--;
    if(_intro_step == 0) _status = 1;
}
void M_lcd::_introGoodBye(){
    switch (_intro_step)
    {
    case 20:
        lcd->setCursor(4,0); lcd->print(F("-")); break;
    case 19:
        lcd->setCursor(4,0); lcd->print(F("G")); break;
    case 18:
        lcd->setCursor(5,0); lcd->print(F("-")); break;
    case 17:
        lcd->setCursor(5,0); lcd->print(F("O")); break;
    case 16:
        lcd->setCursor(6,0); lcd->print(F("-")); break;
    case 15:
        lcd->setCursor(6,0); lcd->print(F("O")); break;
    case 14:
        lcd->setCursor(7,0); lcd->print(F("-")); break;
    case 13:
        lcd->setCursor(7,0); lcd->print(F("D")); break;
    case 12:
        lcd->setCursor(9,0); lcd->print(F("-")); break;
    case 11:
        lcd->setCursor(9,0); lcd->print(F("B")); break;
    case 10:
        lcd->setCursor(10,0); lcd->print(F("-")); break;
    case 9:
        lcd->setCursor(10,0); lcd->print(F("Y")); break;
    case 8:
        lcd->setCursor(11,0); lcd->print(F("-")); break;
    case 7:
        lcd->setCursor(11,0); lcd->print(F("E")); break;
    default:
        break;
    }
    _intro_step--;
    if(_intro_step == 0) _status = 99;
}
void M_lcd::_introChargingStart(){
    lcd->backlight();
    switch (_intro_step)
    {
    case 40:
        lcd->setCursor(0,0); lcd->print(F("-")); break;
    case 39:
        lcd->setCursor(0,0); lcd->print(F("B")); break;
    case 38:
        lcd->setCursor(1,0); lcd->print(F("-")); break;
    case 37:
        lcd->setCursor(1,0); lcd->print(F("A")); break;
    case 36:
        lcd->setCursor(2,0); lcd->print(F("-")); break;
    case 35:
        lcd->setCursor(2,0); lcd->print(F("T")); break;
    case 34:
        lcd->setCursor(3,0); lcd->print(F("-")); break;
    case 33:
        lcd->setCursor(3,0); lcd->print(F("T")); break;
    case 32:
        lcd->setCursor(4,0); lcd->print(F("-")); break;
    case 31:
        lcd->setCursor(4,0); lcd->print(F("E")); break;
    case 30:
        lcd->setCursor(5,0); lcd->print(F("-")); break;
    case 29:
        lcd->setCursor(5,0); lcd->print(F("R")); break;
    case 28:
        lcd->setCursor(6,0); lcd->print(F("-")); break;
    case 27:
        lcd->setCursor(6,0); lcd->print(F("Y")); break;
    case 26:
        lcd->setCursor(8,0); lcd->print(F("-")); break;
    case 25:
        lcd->setCursor(8,0); lcd->print(F("C")); break;
    case 24:
        lcd->setCursor(9,0); lcd->print(F("-")); break;
    case 23:
        lcd->setCursor(9,0); lcd->print(F("H")); break;
    case 22:
        lcd->setCursor(10,0); lcd->print(F("-")); break;
    case 21:
        lcd->setCursor(10,0); lcd->print(F("A")); break;
    case 20:
        lcd->setCursor(11,0); lcd->print(F("-")); break;
    case 19:
        lcd->setCursor(11,0); lcd->print(F("R")); break;
    case 18:
        lcd->setCursor(12,0); lcd->print(F("-")); break;
    case 17:
        lcd->setCursor(12,0); lcd->print(F("G")); break;
    case 16:
        lcd->setCursor(13,0); lcd->print(F("-")); break;
    case 15:
        lcd->setCursor(13,0); lcd->print(F("I")); break;
    case 14:
        lcd->setCursor(14,0); lcd->print(F("-")); break;
    case 13:
        lcd->setCursor(14,0); lcd->print(F("N")); break;
    case 12:
        lcd->setCursor(15,0); lcd->print(F("-")); break;
    case 11:
        lcd->setCursor(15,0); lcd->print(F("G")); break;
    default:
        break;
    }
    _intro_step--;
    if(_intro_step == 0) _status = 4;
}
void M_lcd::refresh(){
    if(first_loop){
        // uint8_t batt_0[] = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x11,0x1F};
    // byte powfont[] = {0x0A,0x0A,0x1F,0x1F,0x1F,0x0E,0x04,0x04};

        //createChar(0, batt_0);
        first_loop = false;
    }
    switch (_status)
    {
    case 0:
        //_intro();
        break;
    case 1:
        //_default_screen();
        if((_param->parameter(PARAM_MENU_SW_LIVE) == 1 || 
            _param->parameter(PARAM_MENU_SW_LIVE) == 2 ||
            _param->parameter(PARAM_MENU_SW_LIVE) == 4) && 
            !_button_pressed ){
                //lcd->clear();
                _clearScreen = true;
                _status = 2;
                _button_pressed = true;
        }
        break;
     case 2:
        _screen_menu_switch();     
        break;   
    default:
        break;
    }
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 0)
        _button_pressed = false;
}

int M_lcd::status(int newStatus){
    if(newStatus >= 0){
        lcd->clear();
        _status = newStatus;
        if(_status == 101){
            _intro_step = 40;
        }else{
            _intro_step = 20;
        }
    }
    return _status;
}

void M_lcd::screenRefresh(){
    switch (_status)
    {
    case 0:
        _intro();
        break;
    case 1:
        _default_screen();
        break;
    case 2:
        _screen_menu();
        break;
    case 3:
        _introGoodBye();
        break;
    case 4:
        this->_charging_screen();
        break;
    case 99:
        _screenOff();
        break;
    case 101:
        this->_introChargingStart();
        break;
    default:
        break;
    }
}
void M_lcd::_default_screen(){
    if(_clearScreen){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print(F(" M U S I C O N  "));
        _clearScreen = false;
    }
    //lcd->setCursor(0,1);
    //lcd->print("Tempo: ");
    lcd->setCursor(0,1);
    // lcd->print((unsigned int)_param->parameter(PARAM_CYCLE_MIN_TIME));
    // lcd->print("-");
    // lcd->print((unsigned int)_param->parameter(PARAM_CYCLE_MAX_TIME));
    // lcd->print("  ");PARAM_SETTER_TACT
    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 0){
        //STOP
        lcd->print("     STOP  ");
    }
    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 1){
        //MOVE
        lcd->print("TACT: ");
        lcd->print((int)_param->parameter(PARAM_SETTER_TACT));
        lcd->print("   ");
    }
    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 2){
        //BRAKE
        lcd->print("    !BRAKE!");
    }
    lcd->setCursor(14,1);
    this->_show_power_icon();
    // lcd->print(sizeof(_menu)/sizeof(_menu[0]));
    //lcd->setCursor(14,1);
}
void M_lcd::_charging_screen(){
    if(_clearScreen){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print(F("BATTERY CHARGING"));
        _clearScreen = false;
    }
    lcd->setCursor(14,1);
    this->_show_power_icon();
}
void M_lcd::_show_power_icon(){
    if(_param->parameter(PARAM_POWER_SUPPLY_OK) == 1) lcd->write(6); else lcd->print(" ");
    int bat_stat = _param->parameter(PARAM_POWER_BAT_STAT); 
    int v_bat = _param->parameter(PARAM_POWER_BAT_V); 
    if(_param->parameter(PARAM_POWER_BAT_CHARGE) == 1){
        if(_charge_icon_delay > 3) _charge_icon_delay = 0;
        if(_charge_icon_delay == 0){
            lcd->write(this->_counter);
            this->_counter++;
            if(this->_counter > 5){
                if(v_bat >= 163) this->_counter = 4;
                if(v_bat < 163 && v_bat >= 150) this->_counter = 3;
                if(v_bat < 150 && v_bat >= 140) this->_counter = 2;
                if(v_bat < 140 && v_bat >= 130) this->_counter = 1;
                if(v_bat < 130) this->_counter = 0;
            }
        }
        _charge_icon_delay++;
    }else{
        if(bat_stat == 100) lcd->write(5);
        if(bat_stat < 100 && bat_stat >= 75) lcd->write(4);
        if(bat_stat < 75 && bat_stat >= 50) lcd->write(3);
        if(bat_stat < 50 && bat_stat >= 25) lcd->write(2);
        if(bat_stat < 25 && bat_stat > 0) lcd->write(1);
        if(bat_stat == 0 && bit1000ms) lcd->write(0); else lcd->print(" "); 
        if(bat_stat < 0) lcd->print(" "); 
    }
}
void M_lcd::_screenOff(){
    lcd->setCursor(0, 0);
    lcd->print("                ");     
    lcd->setCursor(0, 1);
    lcd->print("                ");    
    lcd->noBacklight();
    _status = 100;
}
void M_lcd::_screen_menu_switch(){
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 3 && 
        !_button_pressed ){
            _clearScreen = true;
            _status = 1;
            _menu_display_nr = 0;
            _button_pressed = true;
    }
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 1 && 
        !_button_pressed ){
            // int next_nr = _menu[_menu_display_nr].up;
            if(_is_edit){
                int p = _param->parameter(_menu[_menu_display_nr].parameter) + 1;
                _param->parameter(_menu[_menu_display_nr].parameter, p, true);
            }else{
                _menu_val_diff_dn = -9999;
                _menu_val_diff_up = -9999;                
                if(_menu_display_nr > 0)
                    _menu_display_nr--;
            }
            _button_pressed = true;
    }       
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 2 && 
        !_button_pressed ){
            // int next_nr = _menu[_menu_display_nr].down;
            // int next_nr = next_nr + 1;
            if(_is_edit){
                int p = _param->parameter(_menu[_menu_display_nr].parameter) - 1;
                _param->parameter(_menu[_menu_display_nr].parameter, p, true);
            }else{
                _menu_val_diff_dn = -9999;
                _menu_val_diff_up = -9999;
                if(_menu_display_nr+1 < sizeof(_menu)/sizeof(_menu[0]))
                    _menu_display_nr++;
            }
            _button_pressed = true;
            
    }       
    if(_param->parameter(PARAM_MENU_SW_LIVE) == 4 && 
        !_button_pressed ){
            // int next_nr = _menu[_menu_display_nr].enter;
            _menu_val_diff_dn = -9999;
            _menu_val_diff_up = -9999;
            // if(next_nr >= 0)
            //     _menu_display_nr = next_nr;
            if(_menu[_menu_display_nr].editable){
                _is_edit = !_is_edit;
            }
            _button_pressed = true;
    }
    if((_param->parameter(PARAM_MENU_SW_LIVE) == 1 || _param->parameter(PARAM_MENU_SW_LIVE) == 2) && _param->parameter(PARAM_MENU_SW_TIME) > 1500) {
        _button_pressed = false;
    }
}
void M_lcd::_screen_menu(){
    if(_clearScreen){
        lcd->clear();
        _menu_val_diff_dn = -9999;
        _menu_val_diff_up = -9999;
        _clearScreen = false;
    }
    int size_menu = sizeof(_menu)/sizeof(_menu[0]);
    int up_line = _menu_display_nr;
    int down_line = _menu_display_nr + 1;
    if((_menu_display_nr + 1) == size_menu){
        down_line = -1;
    }
    lcd->setCursor(0,0);
    lcd->print(F(">"));
    lcd->print(F("["));
    lcd->print(_menu[up_line].parameter);
    lcd->print(F("]"));
    if(up_line >= 0){
        if(_menu_val_diff_up - _param->parameter(_menu[up_line].parameter) != 0){
            lcd->setCursor(5,0);
            if(_is_edit){
                lcd->print(F(" ="));
            }else{
                lcd->print(F("->"));
            }
            lcd->setCursor(8,0);
            lcd->print(F("     "));
            lcd->setCursor(8,0);
            float pu = (_param->parameter(_menu[up_line].parameter)*1.00) / _menu[up_line].factor;
            lcd->print(pu,_menu[up_line].precision);
            lcd->print(F("          "));
            if(_menu[up_line].editable){
                lcd->setCursor(15,0);
                lcd->print(F("E"));
            }
            // lcd->setCursor(13,0);
            // lcd->print(_menu[up_line].unit);
        }
        _menu_val_diff_up = _param->parameter(_menu[up_line].parameter);
    }
    lcd->setCursor(0,1);
    if(down_line < 0 || _is_edit){
        lcd->print("                ");
    }else{
        lcd->print(" ");
        lcd->print("[");
        lcd->print(_menu[down_line].parameter);
        lcd->print("]");
        if(_menu[down_line].parameter >= 0){
            if(_menu_val_diff_dn - _param->parameter(_menu[down_line].parameter) != 0){
                lcd->setCursor(5,1);
                lcd->print("->");
                lcd->setCursor(8,1);
                lcd->print("          ");
                lcd->setCursor(8,1);
                float pd = _param->parameter(_menu[down_line].parameter)*1.00 / _menu[down_line].factor;
                lcd->print(pd,_menu[down_line].precision);
                if(_menu[down_line].editable){
                    lcd->setCursor(15,1);
                    lcd->print("E");
                }
                // lcd->setCursor(13,1);
                // lcd->print(_menu[down_line].unit);
            }
            _menu_val_diff_dn = _param->parameter(_menu[down_line].parameter);
        }
    }
}