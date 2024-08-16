#ifndef _M_power_
#define _M_power_

#include "Arduino.h";
#include <M_data.h>;
#include <M_lcd.h>>

// pins:
#define SW_PIN 6
#define ENABLE_PIN 7
#define LED_SW_PIN A1
#define VBATT_PIN A3
#define CSENS_PIN A7
#define BAT_CHARGE_PIN 3

class M_power
{
private:
    // status: (0 - wyłączony, 1 - ładowanie, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
    //          4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię)
    byte _status = 0; 
    M_data* _param;
    M_lcd* _lcd;
    byte led_value = 0;
    bool _motor_test = false;
    bool _sw_switch = false;
    bool _sw_switch_mem = false;
    int _bat_A_smooth[10];
    int _bat_A_smooth_nr = 0;
    int _bat_V_smooth[10];
    int _bat_V_smooth_nr = 0;
    unsigned long _mem_time_led = 0;
    byte _bat_charge_pwm = 50;
    void _check_batt_v();
    void _check_batt_current();
    void _powerManager();
    void _ledPowerButton();
    void _powerSupplyStat();
    void _batStatusCalc();
    void _battery_charging();

public:
    M_power(M_data* param, M_lcd* lcd);
    void refresh();
};

#endif