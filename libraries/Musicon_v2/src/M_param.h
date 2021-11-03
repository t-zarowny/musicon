#ifndef _M_param_
    #define _M_param_

    #include <EEPROM.h>

    #define PARAM_RESTORE           0   // 1001 - parametry zostały przywrócone, każda inna wartość - przywrócenie wartości domyślnych po następnym uruchomieniu
    #define PARAM_POWER_STATUS      1   //0 - wyłączony, 1 - uśpiony i ładuje, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
                                        // 4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię
    #define PARAM_CYCLE_MAX_TIME    2   // czas cyklu mikrokontrolera
    #define PARAM_CYCLE_MIN_TIME    3   // czas cyklu mikrokontrolera
    #define PARAM_MENU_SW_MINUS     4   // punkt ADC przycisku menu "-"
    #define PARAM_MENU_SW_PLUS      5   // punkt ADC przycisku menu "+"
    #define PARAM_MENU_SW_MINUSPLUS 6   // punkt ADC dwóch przycisków jednocześnie "+" i "-"
    #define PARAM_MENU_SW_OK        7   // punkt ADC przycisku menu "OK"
    #define PARAM_MENU_SW_TOLERANCE 8   // tolerancja +- od punktu menu ADC
    #define PARAM_MENU_SW_LIVE_ADC  9   // aktualna wartość wejścia przycisków menu
    #define PARAM_MENU_SW_LIVE      10  // 0-nie wciśnięty żaden przycisk, 1-wcisnięty "+", 
                                        // 2-wciśnięty "-", 3-wciśnięty "+" i "-", 4-wciśnięty OK
    #define PARAM_MENU_SW_TIME      11  // czas przez jaki przycisk jest wciśnięty [s]

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