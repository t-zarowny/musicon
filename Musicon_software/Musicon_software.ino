#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <AS5600.h>
#include <String.h> 
#include <Musicon.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd1(0x3F, 16, 2);


#define hall_pin 4
#define btn_pin 5
#define analogPot_pin 6
#define CSENS_pin A7
#define VBATT_pin A3
#define MENU_SW_pin A2
#define power_sw_pin 6
#define power_pin 7
#define CSN_pin 10            //SPI Chipselect for TMC5160
#define CLOCKOUT_pin 9        //16mhz
#define enable_pin 8          //DRV_ENABLE
#define bat_charge_pin 3       
#define PLED_pin A1 
#define SYS_VOL   5

char nazwa [] = "Musicon"; 
char wersja [] = "v3.4.2"; 

/* Hardware do zmiany:
 *  POT_OUT -> dodać kondensator
 * 
 */

Musicon Musicon(btn_pin, analogPot_pin);
AMS_5600 ams5600;

bool hall_mem = false;
unsigned long t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11 = 0;
bool first_loop = true;
bool power_dn_permit = false;
int csens_mem1,csens_mem2 = 0;
byte batt_100[] = {0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};
byte batt_80[] = {0x0E,0x1B,0x11,0x1F,0x1F,0x1F,0x1F,0x1F};
byte batt_60[] = {0x0E,0x1B,0x11,0x11,0x1F,0x1F,0x1F,0x1F};
byte batt_40[] = {0x0E,0x1B,0x11,0x11,0x11,0x1F,0x1F,0x1F};
byte batt_20[] = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x1F,0x1F};
byte batt_0[] = {0x0E,0x1B,0x11,0x11,0x11,0x11,0x11,0x1F};
byte batt_null[] = {14,27,17,27,21,27,17,31};
byte powfont[] = {0x0A,0x0A,0x1F,0x1F,0x1F,0x0E,0x04,0x04};
bool bit1000ms = true;
bool bit50ms = true;
bool bit50ms_mem = true;
int counter1 = 0;
int tryb_mem = 0;
bool batt_full = false;
bool en_brake = false;
int t_brake = 0;
int licz_max_obc = 0;
int ang, lang = 0;

int temp_czujnik_pradu_v = 0;
int licznik = 0;
unsigned long suma = 0;
int interwal, ostatni_oczyt, napiecie_czujnika_pradu_init, napiecie_czujnika_smoth_init = 0;
int kierunek_obrotu_bebna = true; // 1 - kąt wzrasta, 0 - stop, -1 - kąt maleje,
int kat_obrotu_bebna = 0;
int kat_bebna = 0;
int kat_bebna_mem = 0;
int kat_silnika_mem = 0;
int kat_silnika = 0;
int licznik_pelnych_obr_silnika = 0;
unsigned long czas_pomiaru_mem = 0;
int predkosc_bebna_enkoder = 0;
int predkosc_silnika_enkoder = 0;
int licznik_enkoder_silnik = 0;
long suma_delta_czas = 0;
long suma_delta_kat = 0;

//int analogPinPot = 2;

//int btnUp = 4;
//int btnDown = 3;
//bool btnUp_m = false;
//bool btnDown_m = false;
bool start = false;
int upValue = 0;
int downValue = 0;
float tact = 0;
uint32_t motorSpeed = 0;
uint32_t motorSpeed_mem = 0;
 
int adress_spi_mem = 0;

int SG_RESULT = 0;
int CS_ACTUAL = 0;
int TSTEP = 0;
uint32_t IOIN;

int sg_max = 0;

int warunek = 0;

int odczyt_serial = 999;
int odczyt_serial_par = 0;
boolean koniec_odczytu = false;
int temp10, temp11, temp12 = 0;

//#define MPLXADR 0x70 // Address of TCA9548A I2C Multiplexer
//void multiplexer (uint8_t ch) {
//  if (ch > 1) return;
//  Wire.beginTransmission(MPLXADR);
//  Wire.write(1 << ch);
//  Wire.endTransmission();
//}

   byte error, address;
  int nDevices, ad1, ad2, ad3;

void setup() {
  pinMode(power_pin, OUTPUT);
  digitalWrite(power_pin, HIGH);
  Musicon.parametry->set(52, 3);
  //analogReference( EXTERNAL );
  analogReference( INTERNAL );
  pinMode(CSN_pin,OUTPUT);
  pinMode(bat_charge_pin,OUTPUT);
  pinMode(CLOCKOUT_pin,OUTPUT);
  pinMode(enable_pin, OUTPUT);
  digitalWrite(CSN_pin,HIGH);  
  pinMode(hall_pin, INPUT);
  pinMode(power_sw_pin, INPUT);

  //digitalWrite(8 ,LOW);
  //pinMode(8, INPUT_PULLUP);
  //pinMode(btnDown_pin, INPUT_PULLUP); 


  //set up Timer1
  TCCR1A = bit (COM1A0);                //toggle OC1A on Compare Match
  TCCR1B = bit (WGM12) | bit (CS10);    //CTC, no prescaling
  OCR1A = 0;                            //output every cycle

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);//250khZ spi clk
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  //Wire.setClock(10000);
  Serial.begin(57600);
  
  configTMC5160();
  Musicon.parametry->set(45, 140);  
  Musicon.parametry->set(56, 2);
  Musicon.ctrl->setStopMove();
}
void loop() {
  if(first_loop){
    //Wire.begin();
    Wire.beginTransmission(0x3F);
    if(Wire.endTransmission() == 0){
        lcd = lcd1;
    }
    lcd.begin();   
    lcd.createChar(0, batt_0);
    lcd.createChar(1, batt_20);
    lcd.createChar(2, batt_40);
    lcd.createChar(3, batt_60);
    lcd.createChar(4, batt_80);
    lcd.createChar(5, batt_100);
    lcd.createChar(6, powfont);    
    lcd.createChar(7, batt_null);   
    lcd.backlight();
    Musicon.parametry->set(34, 1);
    first_loop = false;
  }

     t0 = millis();
     Musicon.ctrl->multiplexer(0); 
     updatePot();
     Musicon.ctrl->upd();
    // mediana.in(analogRead(CSENS_pin));
     int roznica = csens_mem1 + csens_mem2;
     roznica = roznica/2;
     roznica = roznica - analogRead(CSENS_pin);
     if(abs(roznica) < 12){
        Musicon.ctrl->addCsens(analogRead(CSENS_pin));
     }
     csens_mem1 = csens_mem2;
     csens_mem2 = analogRead(CSENS_pin);
     
     Musicon.ctrl->addVbat(analogRead(VBATT_pin));
     Musicon.ctrl->addMenuSW(analogRead(MENU_SW_pin));
     //Serial.println(digitalRead(power_sw_pin));  l
     power_manager();
     comm();
 /*     
     Serial.print("P0: ");
     Serial.print(Musicon.parametry->get(0));
    Serial.print(" P1: ");
     Serial.print(Musicon.parametry->get(1));
     Serial.print(" Percent: ");
     Serial.print(Musicon.ctrl->getVelocityPromil());     
     Serial.print(" Pot: ");
     Serial.print(Musicon.ctrl->getPotVal());  
     Serial.print(" RPM: ");
     Serial.println(Musicon.ctrl->getVelocityTmc());       
*/



  //get xactual:
  //Serial.print("\n"); 
  //Serial.print("XActual from TMC5160: "); 
  //sendData_40bit(0x21, 0x0000000, CSN_pin); //read xactual

  //sendData_40bit(0xA0, 0x00000001, CSN_pin); // RAMPMODE = 1 (Positive Velocity mode)
  //sendData_40bit(0xAD, 0x0007D000, CSN_pin);// //XTARGET = 512000 -> 10 revolutions with micro step = 256
  //delay(12000);
  //sendData_40bit(0xAD, 0x00000000, CSN_pin);// //XTARGET = 0 
  //sendData_40bit(0xA0, 0x00000002, CSN_pin); // RAMPMODE = 2 (Negative Velocity mode)
  //delay(12000);
  //sendData_40bit(0x6F, 0x0000000, CSN_pin);
  sendData_40bit(0x6F, 0x00000000, CSN_pin);
  sendData_40bit(0x12, 0x00000000, CSN_pin);

  
  //sendData_40bit(0x35, 0x00000000, CSN_pin);
//  warunek = 53;
    if(Musicon.parametry->get(35) == 1 && Musicon.parametry->get(52) == 3 && !en_brake){
     digitalWrite(enable_pin,LOW);
     //motorSpeed = Musicon.ctrl->getVelocityTmc();
       if(motorSpeed < Musicon.ctrl->getVelocityTmc()){
        motorSpeed = motorSpeed + 500;
       }
       if(motorSpeed > 0 && motorSpeed > Musicon.ctrl->getVelocityTmc()){
        motorSpeed = Musicon.ctrl->getVelocityTmc();
       }
//     if(Musicon.parametry->get(37) == 1){
//        motorSpeed = Musicon.ctrl->getVelocityMaxTmc();
//     }
     if(Musicon.parametry->get(41) > Musicon.parametry->get(76)) en_brake = true;
     
     sendData_40bit(0xA7, motorSpeed, CSN_pin);// VMAX = ....
    }else{
          if(Musicon.parametry->get(41) > Musicon.parametry->get(76) || en_brake){
            digitalWrite(enable_pin,LOW);
            sendData_40bit(0xA7, 1000, CSN_pin);
            en_brake = true;
          }else{
            digitalWrite(enable_pin,HIGH); 
            sendData_40bit(0xA7, 0, CSN_pin);// VMAX = ....
            motorSpeed = 0;            
          }
    }


      //  Serial.print(" mot: ");
      //  Serial.print(motorSpeed);   
      //  Serial.print(", tmc: ");
      //  Serial.println(Musicon.ctrl->getVelocityTmc());
 /*   
    motorSpeed = Musicon.ctrl->getVelocityTmc();
    if(abs(motorSpeed_mem - motorSpeed) > 500){
      sendData_40bit(0xA7, motorSpeed, CSN_pin);// VMAX = ....
      motorSpeed_mem = motorSpeed;
    }
    */
  /*
  if(start){
    Serial.print("ok");
  }else{
    Serial.print("nok");
  }
  Serial.print("TSTEP: ");
  Serial.print(TSTEP);
  Serial.print(" CS_ACTUAL: ");
  Serial.print(CS_ACTUAL);
  Serial.print(", SG_RESULT: ");
  Serial.print(SG_RESULT);
  Serial.print(", Obciazenie: ");
  Serial.print(obciazenie);
  Serial.print(", SG_max: ");
  Serial.print(sg_max);
  Serial.print(", IOIN: ");
  Serial.println(IOIN, HEX);  
  */
  //potValue = analogRead(analogPinPot);
/*
  if(mem_potValue > potValue){
    if(mem_potValue-potValue >3){
      motor();
      mem_potValue = potValue;
    }
  }else{
    if(potValue-mem_potValue >3){
      motor();
      mem_potValue = potValue;
    } 
  }
*/

  timers();
 // hall_read();
/*  
  if(ams5600.detectMagnet() == 1 ){
    //Musicon.parametry->set(68, ams5600.getMagnitude());
    Musicon.parametry->set(68, convertRawAngleToDegrees(ams5600.getRawAngle()));
  }else{
    Musicon.parametry->set(68, -1);
  }
  bool bit50ms = true;
  bool bit50ms_mem = true;
  */
  if(bit50ms && !bit50ms_mem){ 
    //calcMusiconDegree(); 
  }
  bit50ms_mem = bit50ms;
  
//----------------------------------------------
//I2C scanning
 
//  nDevices = 0;
//  ad1=0;
//  ad2=0;
//  ad3=0;
//  for(address = 1; address < 127; address++ )
//  {
//    Musicon.ctrl->multiplexer(0); 
//    Wire.beginTransmission(address);
//    error = Wire.endTransmission();
// 
//    if (error == 0)
//    {
//      nDevices++;
//      if(nDevices == 1)
//        ad1 = address;
//      if(nDevices == 2)
//        ad2 = address;
//      if(nDevices == 3)
//        ad3 = address; 
//      
//    }
//  }
  
//----------------------------------------------
}

float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.87;
  ang = retVal;
  return ang;
}

void updatePot(){
    if(ams5600.detectMagnet() == 1 ){
      int valPot = convertRawAngleToDegrees(ams5600.getRawAngle());
      Musicon.ctrl->setPotVal(valPot);
    }else{
      Musicon.ctrl->setPotVal(-1);
    }
}

void calcMusiconDegree(){
  /*
    bool kierunek_obrotu_bebna = true; // 1 - kąt wzrasta, 0 - kąt maleje
    int kat_obrotu_bebna = 0;
    int kat_silnika_mem = 0;
    int kat_silnika = 0;
    int licznik_pelnych_obr_silnika = 0;
    unsigned long czas_pomiaru_mem = 0;
  */
  if(ams5600.detectMagnet() == 1 ){
    kat_silnika = convertRawAngleToDegrees(ams5600.getRawAngle());
    long temp_kat = 36000 / Musicon.parametry->get(3);
    //int kat_obr_bebna_przy_jednym_obrocie_silnika = 36000 / Musicon.parametry->get(3);
    int kat_obr_bebna_przy_jednym_obrocie_silnika = (int)temp_kat; 
    int licz = true;
    if(kat_silnika < 1000 && kat_silnika_mem > 3000){ 
      kat_silnika_mem = kat_silnika_mem - 3600;
      kat_obrotu_bebna = kat_obrotu_bebna + kat_obr_bebna_przy_jednym_obrocie_silnika;
      licz = false;
    }
    if(kat_silnika > 3000 && kat_silnika_mem < 1000){ 
      kat_silnika_mem = kat_silnika_mem + 3600; 
      kat_obrotu_bebna = kat_obrotu_bebna - kat_obr_bebna_przy_jednym_obrocie_silnika;
      licz = false;
    }
    if(kat_obrotu_bebna > 3600 - kat_obr_bebna_przy_jednym_obrocie_silnika){ 
      kat_obrotu_bebna = 0; 
    }
    if(kat_obrotu_bebna < 0){ 
      kat_obrotu_bebna = 3600 - kat_obr_bebna_przy_jednym_obrocie_silnika;   
    }

    //---------------------
//    int delta_kat = kat_silnika - kat_silnika_mem;
//    int dalta_czas = t0 - czas_pomiaru_mem;
//    if(delta_kat >= 0){ 
//      if(delta_kat == 0){ kierunek_obrotu_bebna = 0; }else{ kierunek_obrotu_bebna = 1; }
//    }else{ 
//      kierunek_obrotu_bebna = -1; 
//    }
    
    kat_bebna = kat_obrotu_bebna + map(kat_silnika, 0, 3600, 0, kat_obr_bebna_przy_jednym_obrocie_silnika);
//
//    predkosc_bebna_enkoder = 50 * delta_kat;
//    dalta_czas = dalta_czas * 3;
//    predkosc_bebna_enkoder = predkosc_bebna_enkoder / dalta_czas;
//int predkosc_silnika_enkoder = 0;
//int licznik_enkoder_silnik = 0;
//int suma_delta_czas = 0;
//int suma_delta_kat = 0;
   //--------------------------------
   
  int dalta_czas = t0 - czas_pomiaru_mem;
  czas_pomiaru_mem = t0;

  int delta_kat = kat_silnika - kat_silnika_mem;

//    long rpm_licznik = delta_kat * 500;
//    long rpm_mianownik = dalta_czas * 3;
//    predkosc_silnika_enkoder = rpm_licznik / rpm_mianownik;

  if(licznik_enkoder_silnik < 4){
    if(licz){
      suma_delta_czas = suma_delta_czas + dalta_czas;
      suma_delta_kat = suma_delta_kat + delta_kat;
      licznik_enkoder_silnik++; 
    }
  }else{
    long rpm_licznik = suma_delta_kat * 500;
    long rpm_mianownik = suma_delta_czas * 3;
    long wynik = rpm_licznik / rpm_mianownik;
    predkosc_silnika_enkoder = (int)wynik * 10;
    predkosc_bebna_enkoder = predkosc_silnika_enkoder / Musicon.parametry->get(3);
    predkosc_bebna_enkoder *= 10;
    suma_delta_czas = 0;
    suma_delta_kat = 0;
    licznik_enkoder_silnik = 0;      
  }    
  


    
    Musicon.parametry->set(67, predkosc_bebna_enkoder);
    int temp_predkosc = abs(predkosc_bebna_enkoder);
    Musicon.parametry->set(41, temp_predkosc);
    Musicon.parametry->set(68, kat_silnika);
    //Musicon.parametry->set(69, predkosc_silnika_enkoder);
    Musicon.parametry->set(69, kat_bebna);
    
    kat_silnika_mem = kat_silnika;
    kat_bebna_mem = kat_bebna;
    
  }else{
    Musicon.parametry->set(68, -1);
  }

  if(Musicon.parametry->get(41) < 50 && Musicon.parametry->get(47) == 0){
    Musicon.parametry->set(59, 0);
  }else{
    Musicon.parametry->set(59, 1);
  }
  
}

void configTMC5160(void)
 {
  sendData_40bit(0x8B, 0x00000000, CSN_pin); //global scaler = 0
  sendData_40bit(0xEC, 0x000100C3, CSN_pin); // CHOPCONF: TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle)
  sendData_40bit(0xED, 0x00070060, CSN_pin); // COLLCONF: SEMIN=2, SEUP=1, SEMAX=10, SEDN=2, SEIMIN=1, SGT=6(12V), SFILT=0
  sendData_40bit(0x80, 0x00000004, CSN_pin);// EN_PWM_MODE=1 enables stealthChop (with default PWM_CONF),
  //sendData_40bit(0xF0, 0xC43E0004, CSN_pin);// 
  
  sendData_40bit(0x90, 0x00040E05, CSN_pin); // IHOLD_IRUN: IHOLD=5, IRUN=14 (max. current), IHOLDDELAY=8
  sendData_40bit(0x91, 0x00000003, CSN_pin);// TPOWERDOWN=3: Delay before power down in stand still
  sendData_40bit(0x93, 0x000001F4, CSN_pin);// TPWMTHRS=500
  sendData_40bit(0x94, 0x00000BB8, CSN_pin);// TCOOLTHRS=5000
  sendData_40bit(0x95, 0x00000190, CSN_pin);// THIGH=2000-7D0 (THIGH=300-12C)
  
  sendData_40bit(0xA3, 0x00000001, CSN_pin);// VSTART = 1
  sendData_40bit(0xA4, 0x000007D0, CSN_pin);// A1 = 5 000 First acceleration
  sendData_40bit(0xA5, 0x0000000A, CSN_pin);// V1 = 26843 Acceleration threshold velocity V1
  sendData_40bit(0xA6, 0x00000E38, CSN_pin);// AMAX = 5000 Acceleration above V1
  sendData_40bit(0xA7, 0x0000000A, CSN_pin);// VMAX = ....
  sendData_40bit(0xA8, 0x000001F4, CSN_pin);// DMAX = 700 Deceleration above V1
  sendData_40bit(0xAA, 0x000001F4, CSN_pin);// D1 = 1400 Deceleration below V1
  sendData_40bit(0xAB, 0x0000000A, CSN_pin);// VSTOP = 10 Stop velocity (Near to zero)
  sendData_40bit(0xA0, 0x00000001, CSN_pin); // RAMPMODE = 1 (Positive Velocity mode)
  sendData_40bit(0xAD, 0x0007D000, CSN_pin);// //XTARGET = 512000 -> 10 revolutions with micro step = 256
  
  // Ready to move!
  }

void hall_read(){
  unsigned long czas = t0-t3;
  long obl_rpm = 0;
  if(digitalRead(hall_pin) == LOW){
      if(hall_mem == false){
        if(czas > 200 && czas <= Musicon.parametry->get(9)){
            Musicon.parametry->set(40, (int)czas);
            obl_rpm =  (long)Musicon.parametry->get(8) * (long)czas ;
            obl_rpm = (long)6000000 / obl_rpm;
            Musicon.parametry->set(41, (int)obl_rpm);
            t3 = millis();
            hall_mem = true;
        }
        if(czas > Musicon.parametry->get(9)){
          Musicon.parametry->set(40, 0);
          Musicon.parametry->set(41, 0);
          t3 = millis();
          hall_mem = true;
        }
      }
  }else{
    hall_mem = false;
  }

    if(czas > Musicon.parametry->get(9)){
        Musicon.parametry->set(40, 0);
        Musicon.parametry->set(41, 0);  
    }

    if(Musicon.parametry->get(40) == 0 && Musicon.parametry->get(47) == 0){
      Musicon.parametry->set(59, 0);
    }else{
      Musicon.parametry->set(59, 1);
    }
  
}

void sendData_40bit(int address, uint32_t datagram, int chipCS) {
 //TMC5160 takes 40 bit data: 8 address and 32 data
 
   //delay(100);
   uint8_t stat;
   unsigned long i_datagram;
   
   digitalWrite(chipCS,LOW);
   delayMicroseconds(10);
   
   stat = SPI.transfer(address);
   
   i_datagram |= SPI.transfer((datagram >> 24) & 0xff);
   i_datagram <<= 8;
   i_datagram |= SPI.transfer((datagram >> 16) & 0xff);
   i_datagram <<= 8;
   i_datagram |= SPI.transfer((datagram >> 8) & 0xff);
   i_datagram <<= 8;
   i_datagram |= SPI.transfer((datagram) & 0xff);
   delayMicroseconds(10);
   digitalWrite(chipCS,HIGH);

   
//   Serial.print("Received: ");
//   PrintHex40(stat, i_datagram);
   //Serial.print(obciazenie);
//   Serial.print(" from register: ");
//   Serial.println(address, HEX);
  
   if(adress_spi_mem == 111){
    if(motorSpeed>0){
      SG_RESULT = i_datagram & 0x3ff;
      Musicon.parametry->set(60, SG_RESULT);
      Musicon.parametry->set(63, min(Musicon.parametry->get(63), SG_RESULT)); 
      sg_max = TSTEP*0.16;
      sg_max = 700-sg_max;
      //obciazenie = map(SG_RESULT, sg_max, 70, 0, 100);
      //if(obciazenie < 0) obciazenie = 0;     
      //if(obciazenie > 100) obciazenie = 100;  
    }
    //else{
      //obciazenie = 0;
   // }
    uint16_t MSB = i_datagram >> 16;
    CS_ACTUAL = MSB & 0x1F;
    Musicon.parametry->set(61, CS_ACTUAL);
   }
  if(adress_spi_mem == 18){
   TSTEP = i_datagram;
   Musicon.parametry->set(62, TSTEP);
  }
  if(adress_spi_mem == warunek){
   IOIN = i_datagram;
  }  
  adress_spi_mem = address;

}
void oblicz_min(){
  if(Musicon.parametry->get(63) < 80 && Musicon.parametry->get(47) > 0){
    licz_max_obc++;
    Musicon.parametry->set(63, 1000);
  }else{
    licz_max_obc = 0;
    Musicon.parametry->set(63, 1000); 
  }
  if(licz_max_obc > Musicon.parametry->get(78)){
    licz_max_obc = 0;
    Musicon.parametry->set(63, 1000);
    Musicon.ctrl->setStopMove();
  }
}


void obciazenieSmoothing(){
  /*
  total = total - readings[readIndex];
  readings[readIndex] = obciazenie;
  total = total + readings[readIndex];
  readIndex++;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }  
  obciazenie_smoth = total / numReadings;
  if(obciazenie_smoth < 0) obciazenie_smoth = 0;     
  if(obciazenie_smoth > 100) obciazenie_smoth = 100;  
  */
}

void PrintHex40(uint8_t stat, uint32_t data) // prints 40-bit data in hex with leading zeroes
{
 char tmp[16];
 uint16_t LSB = data & 0xffff;
 uint16_t MSB = data >> 16;
 sprintf(tmp, "0x%.2X%.4X%.4X", stat, MSB, LSB);
 Serial.print(tmp); 
}
