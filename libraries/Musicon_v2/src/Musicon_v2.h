#ifndef _Musiconv2_h_
#define _Musiconv2_h_
#include <Wire.h>
#include <M_power.h>
#include <M_lcd.h>

class Musicon_v2
{
    private:
        M_power* M_power;
        M_lcd M_lcd;
        
    public:
        void init();
        void main();
};

#endif