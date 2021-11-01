#ifndef _M_param_
    #define _M_param_

    #include <EEPROM.h>
    // Parameters - power module
    #define GROUP_POWER 1001
    #define PARAM_POWER_QUANTITY 10
    #define PARAM_POWER_EPPROM_START 0
    #define PARAM_POWER_RESTORE 0
    #define PARAM_POWER_STATUS 1

    class M_param
    {
    private:
        int _param_power[PARAM_POWER_QUANTITY]  =  { 0 };
        int _param_power_user[PARAM_POWER_QUANTITY]  =  { 0 };

        void _writeParamToEEPROM(int start_address, int param[], int param_nr);
        void _readParamToEEPROM(int start_address, int param[], int param_nr);
        void _init_group(int param_group);
        void _paramCopy(int* src, int* dst, int len);
        void _changeParam(int* param_arr, int* param_arr_user, int eeprom_start, int param_nr, int change_values, bool save_to_eeprom);

    public:
        void init();
        int power(int param_nr);
        void power(int param_nr, int change_values, bool save_to_eeprom);
        void restore(int param_group);
    };

#endif