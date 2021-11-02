#ifndef _Musiconv2_h_
#define _Musiconv2_h_
#include <Wire.h>
#include <M_power.h>
#include <M_lcd.h>
#include <M_param.h>

class Musicon_v2
{
    private:
        M_power* _m_power;
        M_lcd* _m_lcd;
        M_param* _m_param;
        
    public:
        void init();
        void main();
};

#endif