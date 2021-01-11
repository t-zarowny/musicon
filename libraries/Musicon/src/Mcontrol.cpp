#include "Mcontrol.h"

Mcontrol::Mcontrol (int limit_sw_pin, int potentiometer_pin, Mparam * param){
        lim_sw_pin = limit_sw_pin;
        pinMode(lim_sw_pin, INPUT_PULLUP);
        pot_pin = potentiometer_pin; 
        parametry = param;
        for(int i=0; i<8; i++){
                readings_pot[i] = 0;
        }   
        for(int i=0; i<32; i++){
                readings_curr[i] = 0;
        }         
        vbat = new Smoothing30;
        csens = new Smoothing50;
        vcsens = new Smoothing200;
        //batCurr = new Smoothing30;
}
void Mcontrol::upd(){

        if(parametry->get(38) == 0){
                lim_sw_stat = limitSwitchUpd();
                int temp_pot_akt = calcPotVal();
                if(temp_pot_akt != -99){
                      pot_akt_val = temp_pot_akt;  
                }
                        
        }else{
                lim_sw_stat = limitSwitchUpdService();
                pot_akt_val = parametry->get(39);
        }                
        

        
        calcPotLimit();
        setVelocity();
        setPermToStartMove();
        calcEnableMotor();
        updToParam();
        calcCurrVal();
        calcVbatt();
        parametry->set(42, vbat->getVal());
        parametry->set(44, csens->getVal());
        parametry->set(54, vcsens->getVal());
}
void Mcontrol::calcVbatt(){
        //int v_out = parametry->get(42) * (2.58/1023) * 73.8;
        int v_out = parametry->get(42) * (1.1/1023) * 206;
        parametry->set(45, v_out);
}
void Mcontrol::addVbat(int adc){
       vbat->add(adc);
}
void Mcontrol::addCsens(int adc){
       parametry->set(43, adc);
       int cv = adc - 511;
       cv = cv * (1100/1024) * 2;
       vcsens->add(cv); 
       long ca = cv / 0.33;
       csens->add(ca);
       /*
       //int cv = adc * (2.58/1023.0) * 2000;
       int cv = adc * (1.1/1023.0) * 3200;
       vcsens->add(cv);  
       long ca = vcsens->getVal() - parametry->get(19);
       ca *= 1000;
       ca /= 185;
       csens->add(ca);
       */
}
void Mcontrol::addMenuSW(int adc){
        int minus_d = parametry->get(12) - parametry->get(15);
        int minus_u = parametry->get(12) + parametry->get(15);
        int plus_d = parametry->get(13) - parametry->get(15);
        int plus_u = parametry->get(13) + parametry->get(15);     
        int ok_d = parametry->get(14) - parametry->get(15);
        int ok_u = parametry->get(14) + parametry->get(15);            
        
        if(adc > minus_d && adc < minus_u && parametry->get(51) != 1){
                if(parametry->get(49) == 1){
                        parametry->set(parametry->get(50), parametry->get(parametry->get(50))-1);   
                }else{
                        if(parametry->get(48) > 0){
                                parametry->set(48, parametry->get(48)-1);  
                        }
                }
                parametry->set(51, 1); 
        }
        if(adc > plus_d && adc < plus_u && parametry->get(51) != 2){
                if(parametry->get(49) == 1){
                        parametry->set(parametry->get(50), parametry->get(parametry->get(50))+1);   
                }else{
                        if(parametry->get(48) < 80){
                                parametry->set(48, parametry->get(48)+1);  
                        }
                }
                parametry->set(51, 2);    
        }
        if(adc > ok_d && adc < ok_u && parametry->get(51) != 3){
                if(parametry->get(49) == 0 && parametry->get(50) <= 80){
                        parametry->set(49, 1);   
                }else{
                        parametry->set(49, 0); 
                }
                parametry->set(51, 3);  
        }  
        if(adc > 1000){
            parametry->set(51, 0);    
        }                       
       parametry->set(46, adc);

}
int Mcontrol::calcPotVal(){
        int odczyt_akt = analogRead(pot_pin);
        pot_sum = pot_sum + odczyt_akt;
        pot_sum_licz++;
        if(pot_sum_licz >= 5){
                pot_sum_licz = 0;     
                odczyt_akt = pot_sum/5;
                pot_sum = 0;
                int roznica_pot = odczyt_akt - parametry->get(25);
                parametry->set(65, roznica_pot);
                if(abs(roznica_pot) < 10){
                        odczyt_akt = pot_akt_val;     
                }

                total_pot = total_pot - readings_pot[readIndex_pot];
                readings_pot[readIndex_pot] = odczyt_akt;
                total_pot = total_pot + readings_pot[readIndex_pot];
                readIndex_pot = readIndex_pot + 1;
                if (readIndex_pot >= 16) {
                        readIndex_pot = 0;
                }
                return (int)total_pot / (int)16;             
        }else{
          return -99;      
        }


}
void Mcontrol::calcCurrVal(){
      /*   //int curr = (int)parametry->get(43) * (int)10;
        double temp_val = (2.560/1023) * parametry->get(43) * 2000;
        temp_val = 2560.0 - temp_val;
        temp_val = temp_val * 1000.0;
        temp_val = temp_val / 185.0;
        batCurr->add(temp_val); 
        parametry->set(44, batCurr->getVal());
       
        long temp_val = (int)curr - (int)parametry->get(10);
        temp_val = temp_val * (int)parametry->get(11);
        temp_val = temp_val/(int)100;
        int val_ma = (int)temp_val;
        batCurr->add(val_ma); 
        parametry->set(44, batCurr->getVal());
        */
}
int Mcontrol::limitSwitchUpd(){
    //return:  0=button is relased, 2=button is pressed, -1=falling edge, 1=rising edge
        if(digitalRead(lim_sw_pin) == HIGH && lim_sw_mem) return 0;
        if(digitalRead(lim_sw_pin) == HIGH && !lim_sw_mem){  lim_sw_mem = true; return -1; } 
        if(digitalRead(lim_sw_pin) == LOW && lim_sw_mem){  lim_sw_mem = false; return 1; } 
        if(digitalRead(lim_sw_pin) == LOW && !lim_sw_mem) return 2;
}
int Mcontrol::limitSwitchUpdService(){
        if((pot_akt_val  > 0 && pot_akt_val  < 100) && lim_sw_mem) return 0;
        if((pot_akt_val  > 0 && pot_akt_val  < 100) && !lim_sw_mem){  lim_sw_mem = true; return -1; } 
        if((pot_akt_val  == 0 && pot_akt_val  == 100) && lim_sw_mem){  lim_sw_mem = false; return 1; } 
        if((pot_akt_val  == 0 && pot_akt_val  == 100) && !lim_sw_mem) return 2;       
}
void Mcontrol::calcPotLimit(){
        int pot_akt = analogRead(pot_pin);
        if(pot_akt < parametry->get(2) && abs(pot_akt-parametry->get(0)) > 30 && lim_sw_stat == 2){
          parametry->set(0, pot_akt);
        }
        if(pot_akt < parametry->get(2)  && lim_sw_stat == 2){
          parametry->set(36, 1);
        }else{
          parametry->set(36, 0);      
        }   
        if(pot_akt < parametry->get(2)  && lim_sw_stat == 1){
          parametry->set(66, 1);
        }            
        if(pot_akt >= parametry->get(2) && abs(pot_akt-parametry->get(1)) > 30 && lim_sw_stat == 2){
          parametry->set(1, pot_akt);
        }   
        if(pot_akt >= parametry->get(2)  && lim_sw_stat == 2){
          parametry->set(37, 1);
        }else{
          parametry->set(37, 0);      
        }         
}
void Mcontrol::calcEnableMotor(){
        if(perm_to_move && !stop_move){
                enable_motor = true; 
        }else{
                enable_motor = false;
        }       
}
void Mcontrol::updToParam(){
        unsigned int temp = 0;
        parametry->set(25, pot_akt_val);
        parametry->set(26, velocity_promil);
         temp = velocity_tact * 100;
        parametry->set(47, int(velocity_tact));
        parametry->set(27, temp / 1000);
        parametry->set(28, temp % 1000);
         temp = velocity_rpm * 100;
        parametry->set(29, temp / 1000);
        parametry->set(30, temp % 1000);
        parametry->set(31, velocity_tmc / 1000);
        parametry->set(32, velocity_tmc % 1000);       
        parametry->set(33, (int)perm_to_move);
        parametry->set(34, (int)stop_move);
        parametry->set(35, (int)enable_motor);
}
int Mcontrol::getPotVal(){
        return pot_akt_val;
}
void Mcontrol::setVelocity(){
        velocity_promil = map(pot_akt_val, parametry->get(0), parametry->get(1), 0, 1000);
        if(parametry->get(37) == 1){
         velocity_promil = 1000;       
        }
        velocity_promil = constrain(velocity_promil, 0, 1000);
        if(!enable_motor) velocity_promil = 0;

                unsigned int min_tmc = (parametry->get(5) * parametry->get(3))/100;
                min_tmc *= parametry->get(7);
                unsigned int max_tmc = (parametry->get(6) * parametry->get(3))/100;
                max_tmc *= parametry->get(7);     
                velocity_max_tmc = max_tmc;
        velocity_tmc = map(velocity_promil, 0, 1000, min_tmc, max_tmc);
        if(!enable_motor) velocity_tmc = 0;
                float min_rpm = parametry->get(5)*10;
                float max_rpm = parametry->get(6)*10;
        velocity_rpm = map(velocity_promil, 0, 1000, min_rpm, max_rpm);
        velocity_rpm /= 100.0;
        if(!enable_motor) velocity_rpm = 0.00;
                float min_tact = (parametry->get(4) * parametry->get(5))*10.0;
                float max_tact = (parametry->get(4) * parametry->get(6))*10.0;
        velocity_tact = map(velocity_promil, 0.0, 1000.0, min_tact, max_tact);
        velocity_tact /= 100.0;
        if(!enable_motor) velocity_tact = 0.00;
}
void Mcontrol::setPermToStartMove(){
        if(pot_akt_val < parametry->get(2) && lim_sw_stat == 2){
            perm_to_move = true;
            stop_move = true;
        }else{
            stop_move = false;
        }
}
void Mcontrol::setStopMove(){
        perm_to_move = false;
}
int Mcontrol::getVelocityPromil(){
        return velocity_promil;
}
unsigned int Mcontrol::getVelocityTmc(){
        return velocity_tmc;
}
unsigned int Mcontrol::getVelocityMaxTmc(){
        return velocity_max_tmc;
}
float Mcontrol::getVelocityRpm(){
        return velocity_rpm;
}
float Mcontrol::getVelocityTact(){
        return velocity_tact;
}