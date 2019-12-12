#ifndef _Musicon_h_
#define _Musicon_h_

#include "Arduino.h"
#include <WString.h> 

#include "Mparam.h"
#include "Mcontrol.h"

class Musicon {
        private:
        
        

        
        public:
            Mcontrol * ctrl;
            Mparam * parametry;
            Musicon(int ls_pin, int pot_pin);
};

#endif  