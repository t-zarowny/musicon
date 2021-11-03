#ifndef _M_param_
    #define _M_param_

    #include <EEPROM.h>

    #define PARAM_RESTORE 0         // 1001 - parametry zostały przywrócone, każda inna wartość - przywrócenie wartości domyślnych po następnym uruchomieniu
    #define PARAM_POWER_STATUS 1    //0 - wyłączony, 1 - uśpiony i ładuje, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
                                    // 4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię

    class M_param
    {
    private:
        int _parameters[200] = {0};
        void _writeParamToEEPROM(int param_nr, int val);
        void _readParamToEEPROM(int param_nr);
        void _init();
    public:
        M_param();
        int parameter(int param_nr);
        int parameter(int param_nr, int change_values, bool save_to_eeprom = false);
        void restore();
    };

#endif