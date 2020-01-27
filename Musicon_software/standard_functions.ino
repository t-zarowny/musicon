void comm(){
if (Serial.available() > 0) 
        {
            odczyt_serial = Serial.readStringUntil(':').toInt();
            odczyt_serial_par = Serial.readStringUntil('\n').toInt();
            //odczyt_serial.trim();
            if(odczyt_serial == 100){
               for(int i=0; i<80; i++){
                Serial.println(Musicon.parametry->get(i));  
               }      
            }
            if(odczyt_serial == 101){ Serial.println(nazwa); }  
            if(odczyt_serial == 102){ Serial.println(wersja); }   
            if(odczyt_serial<80){
              Musicon.parametry->set(odczyt_serial, odczyt_serial_par);
            }
            /*
            if(odczyt_serial == "ATM_POT_DN"){ Musicon.parametry->set(0, odczyt_serial_par); }  
            if(odczyt_serial == "ATM_POT_UP"){ Musicon.parametry->set(1, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_POT_CT"){ Musicon.parametry->set(2, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_MOT_RT"){ Musicon.parametry->set(3, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_BUT_ROW"){ Musicon.parametry->set(4, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_MIN_RPM"){ Musicon.parametry->set(5, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_MAX_RPM"){ Musicon.parametry->set(6, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_MOT_1RPM"){ Musicon.parametry->set(7, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_NUM_MAG"){ Musicon.parametry->set(8, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_SENS_TIME"){ Musicon.parametry->set(9, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_AIN_CURR0"){ Musicon.parametry->set(10, odczyt_serial_par); } 
            if(odczyt_serial == "ATM_AIN_CURR1"){ Musicon.parametry->set(11, odczyt_serial_par); } 
           
            if(odczyt_serial == "P12"){ Musicon.parametry->set(12, odczyt_serial_par); } 
            if(odczyt_serial == "P13"){ Musicon.parametry->set(13, odczyt_serial_par); } 
            if(odczyt_serial == "P14"){ Musicon.parametry->set(14, odczyt_serial_par); } 
            if(odczyt_serial == "P15"){ Musicon.parametry->set(15, odczyt_serial_par); } 
            if(odczyt_serial == "P17"){ Musicon.parametry->set(17, odczyt_serial_par); } 
            if(odczyt_serial == "P18"){ Musicon.parametry->set(18, odczyt_serial_par); } 
            if(odczyt_serial == "P19"){ Musicon.parametry->set(19, odczyt_serial_par); } 
            if(odczyt_serial == "P20"){ Musicon.parametry->set(20, odczyt_serial_par); } 
            if(odczyt_serial == "P21"){ Musicon.parametry->set(21, odczyt_serial_par); } 
            if(odczyt_serial == "P22"){ Musicon.parametry->set(22, odczyt_serial_par); } 
            if(odczyt_serial == "P23"){ Musicon.parametry->set(23, odczyt_serial_par); } 
            if(odczyt_serial == "P24"){ Musicon.parametry->set(24, odczyt_serial_par); } 
            if(odczyt_serial == "P70"){ Musicon.parametry->set(70, odczyt_serial_par); } 
            if(odczyt_serial == "P71"){ Musicon.parametry->set(71, odczyt_serial_par); } 
            if(odczyt_serial == "P72"){ Musicon.parametry->set(72, odczyt_serial_par); } 
            if(odczyt_serial == "P73"){ Musicon.parametry->set(73, odczyt_serial_par); } 
            if(odczyt_serial == "P74"){ Musicon.parametry->set(74, odczyt_serial_par); } 
            if(odczyt_serial == "P75"){ Musicon.parametry->set(75, odczyt_serial_par); } 
            if(odczyt_serial == "P76"){ Musicon.parametry->set(76, odczyt_serial_par); } 
            if(odczyt_serial == "P77"){ Musicon.parametry->set(77, odczyt_serial_par); } 
            if(odczyt_serial == "P78"){ Musicon.parametry->set(78, odczyt_serial_par); } 
            if(odczyt_serial == "P79"){ Musicon.parametry->set(79, odczyt_serial_par); } 
            */
                                                        
                   
            odczyt_serial = 999;
      } 
}

void lcd_control(){
  if(Musicon.parametry->get(48) == 0){
      int tryb = Musicon.parametry->get(52);
        if(tryb != tryb_mem){
          lcd.clear();
          tryb_mem = tryb;
        }
        if(tryb == 0){
          lcd.setCursor(0, 0);
          lcd.print("                ");     
          lcd.setCursor(0, 1);
          lcd.print("                ");    
          lcd.noBacklight();
        }   
        else if(tryb == 1){
          lcd.backlight();
          lcd.setCursor(0, 0);
          lcd.print("BATTERY CHARGING");     
          lcd.setCursor(0, 1);
          lcd.print("       ");    
          counter1++;
          if(counter1 > 5){
                if(Musicon.parametry->get(53) > 80) counter1=4;
                if(Musicon.parametry->get(53) < 80 && Musicon.parametry->get(53) >= 60) counter1=3;
                if(Musicon.parametry->get(53) < 60 && Musicon.parametry->get(53) >= 45) counter1=2;
                if(Musicon.parametry->get(53) < 45 && Musicon.parametry->get(53) >= 35) counter1=1;
                if(Musicon.parametry->get(53) < 35) counter1=0;
          }
          lcd.write(counter1);
          //lcd.print(Musicon.parametry->get(53));
          lcd.print("          "); 
        }  
        else if(tryb == 2){
          lcd.backlight();
          lcd.setCursor(0, 0);
          lcd.print(" B A T T E R Y  ");     
          lcd.setCursor(0, 1);
          lcd.print(" I S   F U L L  ");    
        }                 
       else if(tryb == 3){
          lcd.backlight();
          if(Musicon.parametry->get(50) < 1000) Musicon.parametry->set(50, 1000);
          lcd.setCursor(0, 0);
          lcd.print(" M U S I C O N  ");
          lcd.setCursor(0, 1);
          if(Musicon.ctrl->getVelocityTact() > 0){
            lcd.print("Tempo: ");
            lcd.print(int(Musicon.ctrl->getVelocityTact()));
            //lcd.print(Musicon.parametry->get(44));
            lcd.print("   ");              
          }else{
            lcd.print("     STOP    "); 
          }

          lcd.setCursor(14, 1);
          if(Musicon.parametry->get(56) == 2) lcd.write(6); else lcd.print(" ");  
          lcd.setCursor(15, 1);
          if(Musicon.parametry->get(45) < 100){
            lcd.write(7);
          }
          else{
                if(Musicon.parametry->get(57) == 1){
                  lcd.write(counter1);
                  counter1++;
                  if(counter1 > 5){
                        if(Musicon.parametry->get(53) > 80) counter1=4;
                        if(Musicon.parametry->get(53) < 80 && Musicon.parametry->get(53) >= 60) counter1=3;
                        if(Musicon.parametry->get(53) < 60 && Musicon.parametry->get(53) >= 45) counter1=2;
                        if(Musicon.parametry->get(53) < 45 && Musicon.parametry->get(53) >= 35) counter1=1;
                        if(Musicon.parametry->get(53) < 35) counter1=0;
                  }
                }else{
                  if(Musicon.parametry->get(53) >= 80) lcd.write(5);
                  if(Musicon.parametry->get(53) < 80 && Musicon.parametry->get(53) >= 60) lcd.write(4);
                  if(Musicon.parametry->get(53) < 60 && Musicon.parametry->get(53) >= 45) lcd.write(3);
                  if(Musicon.parametry->get(53) < 45 && Musicon.parametry->get(53) >= 35) lcd.write(2);
                  if(Musicon.parametry->get(53) < 35 && Musicon.parametry->get(53) >= 25) lcd.write(1);
                  if(Musicon.parametry->get(53) < 25 && bit1000ms) lcd.write(0); else lcd.print(" ");            
                }            
          }


        }
        else if(tryb == 4){
          lcd.backlight();
          lcd.setCursor(0, 0);
          lcd.print("    G O O D     ");     
          lcd.setCursor(0, 1);
          lcd.print("     B Y E      ");    
        } 
        else if(tryb == 5){
          lcd.backlight();
          lcd.setCursor(0, 0);
          lcd.print("     L O W      ");     
          lcd.setCursor(0, 1);
          lcd.print(" B A T T E R Y  ");    
        } 
        
   
  }else{
          Musicon.parametry->set(50, Musicon.parametry->get(48)-1);
          lcd.setCursor(0, 0);
          lcd.print("PARAM ");
          if(Musicon.parametry->get(49) == 1)
              lcd.print("- EDIT:   ");
          else
              lcd.print("- READ:   ");          
          lcd.setCursor(0, 1);
          lcd.print("P_");
          lcd.print(Musicon.parametry->get(50));
          lcd.print(": ");
          lcd.print(Musicon.parametry->get(Musicon.parametry->get(50)));
          lcd.print("        ");
  }
}
void timers(){
  if(t0<1000 || Musicon.parametry->get(52) != 3){
    Musicon.ctrl->setStopMove();
  }
    if(t9 == 0 && t0 > 1000 && t0 < 1100){
      Musicon.parametry->set(55, 0);  
      t9=t0;
  }
    if(t10 == 0 && t0 > 1500 && t0 < 1700){
      int cv = 0;
      for(int i=0; i<40; i++){
        cv += analogRead(CSENS_pin);
        delay(5);
      }
      cv /= 40;
      napiecie_czujnika_smoth_init = cv * (1.1/1023.0) * 3200;
      en_brake = true; 
      t10=t0;    
  }  
    if(t11 == 0 && t0 > 2000 && t0 < 2100){
      int cv = 0;
      for(int i=0; i<40; i++){
        cv += analogRead(CSENS_pin);
        delay(5);
      }
      cv /= 40;
      int cv_temp = cv * (1.1/1023.0) * 3200;
      en_brake = false; 
            

      if(napiecie_czujnika_smoth_init - cv_temp < 20){
        Musicon.parametry->set(19, napiecie_czujnika_smoth_init-10);         
      }else{
        Musicon.parametry->set(19, napiecie_czujnika_smoth_init+12);             
      }
        Musicon.parametry->set(55, Musicon.parametry->get(22));        
        t11=t0;    
  }
/*
  if(t0-t9 > 1){
       //OdczytajNapiecieCzujnikaPradu();
       t9=t0;
  }
  if(t0-t10 > 500){
       long prad = temp_czujnik_pradu_v - 2515;
       prad *= 1000;
       prad /= 185;
       
       Serial.print(analogRead(CSENS_pin));
       Serial.print("\t");
       Serial.print(temp_czujnik_pradu_v);
       Serial.print("\t");
       Serial.print(prad);      
       Serial.print("\t");
       Serial.print(interwal);        
       Serial.println("");
       t10=t0;
  }
*/
   if(t0-t7 > 1000){
       bit1000ms = !bit1000ms; 

       oblicz_min();
       if(en_brake){
        if(t_brake >= Musicon.parametry->get(77) || Musicon.parametry->get(66) == 1){
          Musicon.parametry->set(41, 0);
          en_brake = false;    
          t_brake = 0;
          if(Musicon.parametry->get(66) == 1){
            Musicon.parametry->set(66, 0);
          }
        }else{
          t_brake++;
        }
       }else{
        t_brake = 0;
       }
       t7 = t0;
   }  
   if(Musicon.parametry->get(59) == 0 && Musicon.parametry->get(18) != 0 && Musicon.parametry->get(52) == 3){
       unsigned long temp = (unsigned long) Musicon.parametry->get(18)*1000UL;
       if(t0-t8 > temp){
          Musicon.parametry->set(52, 4); 
       }
   }else{
       t8 = t0;
   }
   if(t0-t1 > Musicon.parametry->get(16)){

       lcd_control();
       t1 = t0;
   }
   if(Musicon.parametry->get(51) == 1 || Musicon.parametry->get(51) == 2){
       if(t0-t2 > Musicon.parametry->get(16)){
           Musicon.parametry->set(51, 0);
           t2 = t0;
       }
   }else{
       t2 = t0;
   }
   
   if((Musicon.parametry->get(52) == 4 || Musicon.parametry->get(52) == 5 || Musicon.parametry->get(52) == 2) && t0 > 3000){
       if(t0-t4 > Musicon.parametry->get(17)){
           Musicon.parametry->set(52, 0);
           Musicon.ctrl->setStopMove();
           digitalWrite(power_pin, LOW);
       }
   }else{
       t4 = t0;
   } 
   if(t0-t6 > Musicon.parametry->get(21)){
       battery_charging();
       t6 = t0;
   }

   /*
   if(obciazenie_smoth < 85){
      t2 = t0;
   }else{
      if(t0-t2 > 2000){
        start = false;
      }
   }
   */
}
void power_manager(){
     if(Musicon.parametry->get(52) == 3 && digitalRead(power_sw_pin) == HIGH){
      
        if(power_dn_permit){
          Musicon.ctrl->setStopMove();
          Musicon.parametry->set(52, 4);
          Musicon.parametry->set(48, 0);
        }
     }else{
      power_dn_permit = true;
     }
     if(Musicon.parametry->get(52) == 0 && Musicon.parametry->get(57) == 1){
      Musicon.parametry->set(52, 1);
     }
     if(Musicon.parametry->get(52) == 1 && Musicon.parametry->get(57) == 3){
      Musicon.parametry->set(52, 2);
     }
     if((Musicon.parametry->get(52) == 0 || Musicon.parametry->get(52) == 1) && (digitalRead(power_sw_pin) == HIGH || Musicon.parametry->get(41) > 500)){
      Musicon.parametry->set(52, 3);
      digitalWrite(power_pin, HIGH);
      power_dn_permit = false;
      batt_full = false;
      t4 = t0;
      t8 = t0;
     }  
}
void battery_charging(){
  if(Musicon.parametry->get(44) < Musicon.parametry->get(75) && t0 > 10000 && Musicon.parametry->get(45) > 90){
        Musicon.parametry->set(56, 1);
        Musicon.parametry->set(55, Musicon.parametry->get(22));
        Musicon.parametry->set(57, 0);
  }else{
        Musicon.parametry->set(56, 2);
        if((Musicon.parametry->get(55) < Musicon.parametry->get(23)) && (Musicon.parametry->get(44) < Musicon.parametry->get(20)-100)){
          Musicon.parametry->set(55, Musicon.parametry->get(55)+1);
        }
        else if((Musicon.parametry->get(55) > 0) && (Musicon.parametry->get(44) > Musicon.parametry->get(20)+100)){
          Musicon.parametry->set(55, Musicon.parametry->get(55)-1);
        }
        analogWrite(bat_charge_pin, Musicon.parametry->get(55));    
        if((Musicon.parametry->get(55) == Musicon.parametry->get(23) && Musicon.parametry->get(44) < Musicon.parametry->get(70)) || batt_full){
          Musicon.parametry->set(57, 3);
          if(Musicon.parametry->get(44) < Musicon.parametry->get(70)) batt_full = true; else batt_full = false;
        }else{
          if(Musicon.parametry->get(44) >= Musicon.parametry->get(70))
            Musicon.parametry->set(57, 1);
        }
  }
  int temp = 0;
  if(Musicon.parametry->get(56) == 1 && Musicon.parametry->get(47) == 0){
    temp = map(Musicon.parametry->get(45),Musicon.parametry->get(71),Musicon.parametry->get(72)-Musicon.parametry->get(73),0,100);
      temp = constrain(temp, 0, 100);
      if((Musicon.parametry->get(52) == 1 && temp >= Musicon.parametry->get(53)) || Musicon.parametry->get(52) > 1)
        Musicon.parametry->set(53, temp);    
      if(Musicon.parametry->get(52) == 3 && Musicon.parametry->get(53) < 5 && t0 > 10000){
      Musicon.parametry->set(52, 5);
     }  
  }
  else if(Musicon.parametry->get(56) == 1 && Musicon.parametry->get(47) > 0){
    temp = map(Musicon.parametry->get(45),Musicon.parametry->get(71),Musicon.parametry->get(72)-Musicon.parametry->get(74),0,100);
      temp = constrain(temp, 0, 100);
      if(Musicon.parametry->get(52) == 1 && temp >= Musicon.parametry->get(53) || Musicon.parametry->get(52) > 1)
        Musicon.parametry->set(53, temp);
      if(Musicon.parametry->get(52) == 3 && Musicon.parametry->get(53) < 5){
        Musicon.parametry->set(64, Musicon.parametry->get(64)+1);
     }else{
        Musicon.parametry->set(64, 0);
     }
      if(Musicon.parametry->get(64) == Musicon.parametry->get(79) && t0 > 10000){
        Musicon.parametry->set(52, 5);
        Musicon.parametry->set(64, 0);
      }
  }else{
    temp = map(Musicon.parametry->get(45),Musicon.parametry->get(71),Musicon.parametry->get(72),0,100);
    temp = constrain(temp, 0, 100);
    Musicon.parametry->set(53, temp);
  }
  
 
}
