#ifndef _Musiconv2_h_
#define _Musiconv2_h_
#include <Wire.h>
#include <M_power.h>
#include <M_lcd.h>
#include <M_param.h>

#define MENU_SW_PIN A2

class Musicon_v2
{
    private:
        unsigned long _time_cycle_last_us = 0;
        int _time_cycle_max_us = 0;
        int _time_cycle_min_us = 32000;
        unsigned long _time_act_us = 0;
        unsigned long _time_start_menu_sw_ms = 0;
        int _count_cycle = 0;
        M_power* _m_power;
        M_lcd* _m_lcd;
        M_param* _m_param;
        void _cycleTimeCheck();
        void _menu_sw_control();
        
    public:
        void init();
        void main();
};

#endif