#ifndef _M_lcd_
    #define _M_lcd_
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>
    #include <arduino.h>
    #include <M_param.h>
     
    #define LCD_ADRESS_1 0x27
    #define LCD_ADRESS_2 0x3F

class M_lcd
{
private:
    uint8_t _adress = 0;
    void _checkAdress(uint8_t adr);
    LiquidCrystal_I2C* lcd;
    M_param* _param;
    void M_lcd::intro();
    int _intro_step = 15;
    int _status = 0; //0-intro; 1-default
    void default_screen();
public:
    M_lcd(M_param* param);
    void init();
    void refresh();
};



#endif