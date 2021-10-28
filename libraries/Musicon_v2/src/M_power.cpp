#include <M_power.h>

void M_power::init(){
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, HIGH);
    analogWrite(LED_SW_PIN, 255);
}