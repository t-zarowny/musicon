#ifndef _M_power_
#define _M_power_

#include "Arduino.h";
#include <M_param.h>;

// pins:
#define SW_PIN 6
#define ENABLE_PIN 7
#define LED_SW_PIN A1
#define VBATT_PIN A3
#define CSENS_PIN A7


class M_power
{
private:
    // status: (0 - wyłączony, 1 - ładowanie, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
    //          4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię)
    byte _status = 0; 
    M_param* _param;
    void _check_batt_v();
    void _check_batt_current();

public:
    M_power(M_param* param);
    void refresh();
};

#endif