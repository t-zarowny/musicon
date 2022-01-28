#ifndef _Mmotor_h_
#define _Mmotor_h_
#include <SPI.h>
#include <M_data.h>

#define CSN_PIN 10            //SPI Chipselect for TMC5160
#define CLOCKOUT_PIN 9        //16mhz
#define ENABLE_PIN 8          //DRV_ENABLE

class M_motor
{
private:
    M_data* _param;
    unsigned long _mem_time_brake_start = 0;
    unsigned long _mem_time_brake_vel_start = 0;
    bool _brake = false;
    int _adress_spi_mem = 0;
    bool _enable_motor = false;
    uint32_t _motorSpeed = 0;
    int SG_RESULT = 0;
    int CS_ACTUAL = 0;
    int TSTEP = 0;
    void _configTMC5160();
    void _sendData_40bit(int address, uint32_t datagram, int chipCS);
    void _velocitySet();
    void _velocityControl();
public:
    M_motor(M_data* param);
    void refresh();
    bool init_ok = false;
};




#endif