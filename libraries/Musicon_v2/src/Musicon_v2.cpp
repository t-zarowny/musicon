#include <Musicon_v2.h>

void Musicon_v2::init(){

    Serial.begin(57600);
    Wire.begin();
    M_power->init();
    //M_lcd M_lcd;
    M_lcd.init();
    // Serial.print("Adres: ");
    // Serial.println(M_lcd.getAdress());
    // Serial.println(0x27);
}
 
void Musicon_v2::main(){

}