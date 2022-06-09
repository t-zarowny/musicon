#ifndef _Musiconv2_h_
#define _Musiconv2_h_
#include <Wire.h>
#include <M_power.h>

#include <M_lcd.h>
#include <M_data.h>
#include <M_motor.h>
#include <AS5600.h>

#define MENU_SW_PIN A2
#define LIMIT_SWITCH_PIN 5
#define MPLXADR 0x70


class Musicon_v2
{
    private:
        unsigned long _time_cycle_last_us = 0;
        unsigned long _time_cycle_max_us = 0;
        unsigned long _time_cycle_min_us = 32000;
        unsigned long _time_act_us = 0;
        unsigned long _time_start_menu_sw_ms = 0;
        unsigned long _time_order_subroutines = 0;
        unsigned long _time_mem_hall = 0;
        unsigned long _time_last_read_motor_hall = 0;
        unsigned long _time_mem_bit1000ms = 0;
        float _angle_last_read_motor_hall = 9999.0;
        float _velocity_mem = 0.0;
        int _velocity_mem_count = 0;
        float _motor_velocity_smooth[3] = { 100 };
        int _motor_velocity_smooth_count = 0;
        int _count_cycle = 0;
        int _setter_tact_mem = 0;
        bool _lim_sw_mem;
        bool _setter_hall_init_ok = false;
        M_power* _m_power;
        M_lcd* _m_lcd;
        LiquidCrystal_I2C* _lcd;
        M_data* _m_param;
        M_motor* _m_motor;
        AMS_5600 _ams5600;
        int _nextSubroutines = 0;
        bool _init_ok = false;
        void _cycleTimeCheck();
        void _menu_sw_control();
        void _menu_sw_press_time(unsigned long start_time);
        int _multiplexer (uint8_t ch);
        void _clickSetterLimitSwitch();
        int _limitSwitchUpd();
        void _tactCalculate();
        float _convertRawAngleToDegrees(word newAngle);
        void _orderSubroutines();
        void _readSetterHall();
        void _readMotorHall();
        
    public:
        void init(LiquidCrystal_I2C* lcd);
        void main();
        Musicon_v2();
        ~Musicon_v2();
};

#endif