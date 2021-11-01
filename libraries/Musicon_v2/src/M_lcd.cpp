#include <M_lcd.h>

void M_lcd::init(){
    _checkAdress(LCD_ADRESS_1);
    _checkAdress(LCD_ADRESS_2);
    if(_adress != 0){
        LiquidCrystal_I2C lcd(_adress, 16, 2);

        lcd.begin(); 
        //lcd->clear();
        //lcd->setCursor(0,0);
        lcd.print("In OK2");
    }
}
void M_lcd::_checkAdress(uint8_t adr){
    Wire.beginTransmission(adr);
    if(Wire.endTransmission() == 0) 
        _adress = adr;
}

uint8_t M_lcd::getAdress(){
    return _adress;
}