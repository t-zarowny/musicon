
#include <Musicon_v2.h>

Musicon_v2 Musicon;
// M_data Data;
// M_lcd lcd;

uint8_t _adress = 0;
LiquidCrystal_I2C* lcd;
byte batt_100[] = {0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
byte batt_80[] = {0x0E,0x1B,0x11,0x1F,0x1F,0x1F,0x1F,0x1F};
byte batt_60[] = {0x0E,0x1B,0x11,0x11,0x1F,0x1F,0x1F,0x1F};
byte batt_40[] = {0x0E,0x1B,0x11,0x11,0x11,0x1F,0x1F,0x1F};
byte batt_20[] = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x1F,0x1F};
byte batt_0[] = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x11,0x1F};
byte powfont[] = {0x0A,0x0A,0x1F,0x1F,0x1F,0x0E,0x04,0x04};



void setup()
{
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
    _checkAdress(LCD_ADRESS_1);
    _checkAdress(LCD_ADRESS_2);
    if(_adress != 0){
      LiquidCrystal_I2C _lcd(_adress, 16, 2);

        _lcd.begin(); 
        _lcd.createChar(0, batt_0);
        _lcd.createChar(1, batt_20);
        _lcd.createChar(2, batt_40);
        _lcd.createChar(3, batt_60);
        _lcd.createChar(4, batt_80);
        _lcd.createChar(5, batt_100);
        _lcd.createChar(6, powfont);
        lcd->clear();
        lcd = &_lcd;
    }
        Musicon.init(lcd);

}
void loop()
{
    Musicon.main();
}
void _checkAdress(uint8_t adr){
    Wire.beginTransmission(adr);
    if(Wire.endTransmission() == 0) 
        _adress = adr;
}
