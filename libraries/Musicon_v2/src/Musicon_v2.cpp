#include <Musicon_v2.h>

void Musicon_v2::init(){
    _m_param = new M_param();
    Serial.begin(57600);
    Wire.begin();
    _m_power->init();
    _m_lcd = new M_lcd(_m_param);
    // Serial.print("Adres: ");
    // Serial.println(M_lcd.getAdress());
    // Serial.println(0x27);
}
 
void Musicon_v2::main(){
    _m_lcd->update();
}