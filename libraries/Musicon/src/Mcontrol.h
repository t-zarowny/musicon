
#ifndef _M_control_h_
#define _M_control_h_
#define MPLXADR 0x70
#include "Arduino.h"
#include "Mparam.h"
#include "Smoothing.h"
#include "AS5600.h"
#include "Wire.h"

class Mcontrol
{
  private:
        int pot_pin=0;
        int lim_sw_pin=0;
        int pot_akt_val=0;
        int pot_sum = 0;
        int pot_sum_licz = 0;
        int readings_pot[16] = { 0 };   
        int readIndex_pot = 0;              
        long total_pot = 0;         
        int readings_curr[32];   
        int readIndex_curr = 0;              
        long total_curr = 0;               
        int lim_sw_stat=0;
        bool lim_sw_mem = false;
        int ang1 = 0;
        int calcPotVal();
        Mparam * parametry;
        int velocity_promil=0;
        unsigned int velocity_tmc=0;
        unsigned int velocity_max_tmc=0;
        float velocity_rpm=0;
        float velocity_tact=0;
        bool perm_to_move = false;
        bool stop_move = true;
        bool enable_motor = false;
        int limitSwitchUpd();
        void setVelocity();
        void setPermToStartMove();
        void calcPotLimit();
        int limitSwitchUpdService();
        void calcEnableMotor();
        void updToParam();
        void calcCurrVal();
        void calcVbatt();
        Smoothing30 * vbat;
        Smoothing50 * csens;
        //Smoothing30 * batCurr;
        Smoothing200 * vcsens;
        float smoothedVal = 0.0;
        float smoothStrength = 4.0;
        AMS_5600 ams;
        float convRawAngleToDegrees(word newAngle);
        
        
  public:
        Mcontrol (int limit_sw_pin, int potentiometer_pin, Mparam * param);
        void upd();
        int getPotVal();
        int setPotVal(int pot);
        int getVelocityPromil();
        unsigned int getVelocityTmc();
        unsigned int getVelocityMaxTmc();
        float getVelocityRpm();
        float getVelocityTact();
        void setStopMove();
        void addVbat(int adc);
        void addCsens(int adc);
        void addMenuSW(int adc);
        void multiplexer (uint8_t ch);
        
};

#endif