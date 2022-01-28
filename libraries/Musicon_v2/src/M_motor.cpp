#include <M_motor.h>


M_motor::M_motor(M_data* param)
{
  _param = param;
  pinMode(CSN_PIN,OUTPUT);
  pinMode(CLOCKOUT_PIN,OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(CSN_PIN,HIGH);  
  //set up Timer1
  TCCR1A = bit (COM1A0);                //toggle OC1A on Compare Match
  TCCR1B = bit (WGM12) | bit (CS10);    //CTC, no prescaling
  OCR1A = 0;                            //output every cycle

  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV64);//250khZ spi clk
  SPI.setDataMode(SPI_MODE3);
  SPI.begin();
  _configTMC5160();
}

void M_motor::_configTMC5160(){
  _sendData_40bit(0x8B, 0x00000000, CSN_PIN); //global scaler = 0
  _sendData_40bit(0xEC, 0x000100C3, CSN_PIN); // CHOPCONF: TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle)
  _sendData_40bit(0xED, 0x00070060, CSN_PIN); // COLLCONF: SEMIN=2, SEUP=1, SEMAX=10, SEDN=2, SEIMIN=1, SGT=6(12V), SFILT=0
  _sendData_40bit(0x80, 0x00000004, CSN_PIN);// EN_PWM_MODE=1 enables stealthChop (with default PWM_CONF),
  //sendData_40bit(0xF0, 0xC43E0004, CSN_PIN);// 
  
  _sendData_40bit(0x90, 0x00040E05, CSN_PIN); // IHOLD_IRUN: IHOLD=5, IRUN=14 (max. current), IHOLDDELAY=8
  _sendData_40bit(0x91, 0x00000003, CSN_PIN);// TPOWERDOWN=3: Delay before power down in stand still
  _sendData_40bit(0x93, 0x000001F4, CSN_PIN);// TPWMTHRS=500
  _sendData_40bit(0x94, 0x00000BB8, CSN_PIN);// TCOOLTHRS=5000
  _sendData_40bit(0x95, 0x00000190, CSN_PIN);// THIGH=2000-7D0 (THIGH=300-12C)
  
  _sendData_40bit(0xA3, 0x00000001, CSN_PIN);// VSTART = 1
  _sendData_40bit(0xA4, 0x0000000A, CSN_PIN);// A1 = 5 000 First acceleration
  _sendData_40bit(0xA5, 0x0000000A, CSN_PIN);// V1 = 26843 Acceleration threshold velocity V1
  _sendData_40bit(0xA6, 0x00000E38, CSN_PIN);// AMAX = 5000 Acceleration above V1
  _sendData_40bit(0xA7, 0x0000000A, CSN_PIN);// VMAX = ....
  _sendData_40bit(0xA8, 0x000001F4, CSN_PIN);// DMAX = 700 Deceleration above V1
  _sendData_40bit(0xAA, 0x000001F4, CSN_PIN);// D1 = 1400 Deceleration below V1
  _sendData_40bit(0xAB, 0x0000000A, CSN_PIN);// VSTOP = 10 Stop velocity (Near to zero)
  _sendData_40bit(0xA0, 0x00000001, CSN_PIN); // RAMPMODE = 1 (Positive Velocity mode)
  _sendData_40bit(0xAD, 0x0007D000, CSN_PIN);// //XTARGET = 512000 -> 10 revolutions with micro step = 256
  
  //_sendData_40bit(0xA7, 47790, CSN_PIN);// VMAX = ....
  // Ready to move!
}

void M_motor::_sendData_40bit(int address, uint32_t datagram, int chipCS){
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

//_param->parameter(PARAM_MOTOR_PERM_TO_MOVE, _adress_spi_mem);
  
   if(_adress_spi_mem == 111){

      SG_RESULT = i_datagram & 0x3ff;
      _param->parameter(PARAM_MOTOR_TMC_SG_RESULT, SG_RESULT);
      //Musicon.parametry->set(60, SG_RESULT);
            // Musicon.parametry->set(63, min(Musicon.parametry->get(63), SG_RESULT)); 
            // sg_max = TSTEP*0.16;
            // sg_max = 700-sg_max;
      //obciazenie = map(SG_RESULT, sg_max, 70, 0, 100);
      //if(obciazenie < 0) obciazenie = 0;     
      //if(obciazenie > 100) obciazenie = 100;  

    //else{
      //obciazenie = 0;
   // }
    uint16_t MSB = i_datagram >> 16;
    CS_ACTUAL = MSB & 0x1F;
    _param->parameter(PARAM_MOTOR_TMC_CS_ACTUAL, CS_ACTUAL);
    //Musicon.parametry->set(61, CS_ACTUAL);
   }
  if(_adress_spi_mem == 18){
   TSTEP = i_datagram;
   _param->parameter(PARAM_MOTOR_TMC_TSTEP, TSTEP);
   //Musicon.parametry->set(62, TSTEP);
  }
  _adress_spi_mem = address;

}

void M_motor::refresh(){
    if(_brake){
      _param->parameter(PARAM_MOTOR_PERM_TO_MOVE, 2);
    }else{
      if((_param->parameter(PARAM_SETTER_TACT) == 32 && _param->parameter(PARAM_SETTER_LIM_SW_STAT) == 2) 
          || !init_ok || _param->parameter(PARAM_POWER_STATUS) != 3){
          _param->parameter(PARAM_MOTOR_PERM_TO_MOVE, 0);
      }else{
          _param->parameter(PARAM_MOTOR_PERM_TO_MOVE, 1);
      }
    }
    _sendData_40bit(0x6F, 0x00000000, CSN_PIN);
    _sendData_40bit(0x12, 0x00000000, CSN_PIN);

    _velocitySet();
    _velocityControl();
}
void M_motor::_velocitySet(){
    unsigned int min_tmc = (_param->parameter(PARAM_MOTOR_TMC_MIN_VEL) * _param->parameter(PARAM_MOTOR_RATIO))/100;
    min_tmc *= _param->parameter(PARAM_MOTOR_TMC_VELOCITY);
    unsigned int max_tmc = (_param->parameter(PARAM_MOTOR_TMC_MAX_VEL) * _param->parameter(PARAM_MOTOR_RATIO))/100;
    max_tmc *= _param->parameter(PARAM_MOTOR_TMC_VELOCITY);   

    
    uint32_t motorSpeed = 0;

    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 1){
      _enable_motor = true;
      motorSpeed = map(_param->parameter(PARAM_SETTER_TACT),32,144,min_tmc,max_tmc);
    }else{
      _enable_motor = false;
      motorSpeed = 0;
    }
    if(_param->parameter(PARAM_MOTOR_PERM_TO_MOVE) == 2){
      _enable_motor = true;
      motorSpeed = 1000;
    }

    if(_enable_motor){
      digitalWrite(ENABLE_PIN,LOW);
    }else{
      digitalWrite(ENABLE_PIN,HIGH);
    }

    _sendData_40bit(0xA7, motorSpeed, CSN_PIN);// VMAX = ....
}

void M_motor::_velocityControl(){
  unsigned long actual_time = millis();

  // Jeśli prędkość aktualna jest mniejsza od prędkości hamowania, 
  // to przepisuj aktualny czas do _mem_time_brake_vel_start
  if((abs(_param->parameter(PARAM_MOTOR_VELOCITY)) < _param->parameter(PARAM_MOTOR_BRAKE_VEL))){
    _mem_time_brake_vel_start = actual_time;
  }
  // Sprawdź, czy ostatni zapisany czas nieprzekraczania prędkości nie jest większy niż zadany w parametrach
  // Jeśli tak, to rozpocznij liczyć czas hamowania i włącz hamowanie
  if((actual_time - _mem_time_brake_vel_start) > _param->parameter(PARAM_MOTOR_BRAKE_TIME)){
    if(_mem_time_brake_start == 0) {
      _mem_time_brake_start = actual_time;
      _brake = true;
    }
  }
  // Jeśli załączone jest hamowanie, po upływie 2 sekund wyłącz
  if(_brake && ((actual_time - _mem_time_brake_start) > 2000)){
      _mem_time_brake_start = 0;
      _brake = false;
  }



}