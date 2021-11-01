#ifndef _M_lcd_
    #define _M_lcd_
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>
     
    #define LCD_ADRESS_1 0x27
    #define LCD_ADRESS_2 0x3F

class M_lcd
{
private:
    uint8_t _adress = 0;
    void _checkAdress(uint8_t adr);
    LiquidCrystal_I2C* lcd;
public:
    void init();
    uint8_t getAdress();
};

#endif