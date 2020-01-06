
#ifndef _M_par_
#define _M_par_
#include "EEPROM.h"

class Mparam
{
  private:
        int param[80];
        void init(int nr, int def);
        void save(int nrParam, int val);
        int read(int nrParam);
        void setNazwa(char n[8]);
        
  public:
        //char  nazwa [8];
        //char wersja [7];
        Mparam();
        int get(int nrParam);
        void set(int nrParam, int valParam); 
};
#endif