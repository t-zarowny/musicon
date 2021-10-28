#ifndef _M_param_
    #define _M_param_

    // Parameters - power module
    #define PARAM_POWER_QUANTITY 10
    #define PARAM_POWER_EPPROM_START 0
    #define PARAM_POWER_STATUS 0

    class M_param
    {
    private:
        int _param_power[PARAM_POWER_QUANTITY];
        int _param_power_user[PARAM_POWER_QUANTITY];
        int _param_power_default[PARAM_POWER_QUANTITY];

        void _writeParamToEEPROM();
        void _readParamToEEPROM();

    public:
        int param_power(int param_nr, int change_values = NULL);
        void param_power_restore();
    };

#endif