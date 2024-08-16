#include "Musicon.h"

Musicon::Musicon(int ls_pin, int pot_pin){
    parametry = new Mparam;
    ctrl = new Mcontrol(ls_pin, pot_pin, parametry);
}
