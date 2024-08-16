#pragma once
#ifndef _M_lcd_
    #define _M_lcd_
    // #include <Wire.h>
    
    // #include <hd44780.h>                       
    // #include <hd44780ioClass/hd44780_I2Cexp.h> 
    #include <arduino.h>
    #include <M_data.h>
    #include "M_menu.cpp"
    #include <LiquidCrystal_I2C.h>
    #define LCD_ADRESS_1 0x27
    #define LCD_ADRESS_2 0x3F

class M_lcd
{
private:
    uint8_t _adress = 0;
    // hd44780_I2Cexp* lcd1;
    LiquidCrystal_I2C* lcd;
    M_data* _param;
    bool first_loop = true;
    byte powfont[8];
    int _intro_step = 20;
    int _status = 0; //0-intro; 1-default screen; 2-menu screen; 3-good bye; 
                     //99-OFF; 100-OFF_OK; 4-charging; 5-end charging
    bool _button_pressed = false;
    int _menu_display_nr = 0;
    int _menu_val_diff_up = 0;
    int _menu_val_diff_dn = 0;
    bool _is_edit = false;
    bool _clearScreen = false;
    int _counter = 0;
    int _charge_icon_delay = 0;
    void _intro();
    void _default_screen();
    void _screen_menu();
    void _screen_menu_switch();
    void _menu_control();
    void _introGoodBye();
    void _screenOff();
    void _show_power_icon();
    void _charging_screen();
    void _introChargingStart();
public:
    bool bit1000ms = false;
    M_lcd(M_data* param, LiquidCrystal_I2C* l);
    M_lcd();
    void refresh();
    void screenRefresh();
    int status(int newStatus = -1);
};

#endif