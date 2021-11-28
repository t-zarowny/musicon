#ifndef _M_param_
    #define _M_param_

    #include <EEPROM.h>

    #define PARAM_RESTORE               0   // 1001 - parametry zostały przywrócone, każda inna wartość - przywrócenie wartości domyślnych po następnym uruchomieniu
    #define PARAM_POWER_STATUS          1   //0 - wyłączony, 1 - uśpiony i ładuje, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
                                        // 4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię
    #define PARAM_CYCLE_MAX_TIME        2   // czas cyklu mikrokontrolera [us]
    #define PARAM_CYCLE_MIN_TIME        3   // czas cyklu mikrokontrolera [us]
    #define PARAM_MENU_SW_MINUS         4   // punkt ADC przycisku menu "-"
    #define PARAM_MENU_SW_PLUS          5   // punkt ADC przycisku menu "+"
    #define PARAM_MENU_SW_MINUSPLUS     6   // punkt ADC dwóch przycisków jednocześnie "+" i "-"
    #define PARAM_MENU_SW_OK            7   // punkt ADC przycisku menu "OK"
    #define PARAM_MENU_SW_TOLERANCE     8   // tolerancja +- od punktu menu ADC
    #define PARAM_MENU_SW_LIVE_ADC      9   // aktualna wartość wejścia przycisków menu
    #define PARAM_MENU_SW_LIVE          10  // 0-nie wciśnięty żaden przycisk, 1-wcisnięty "+", 
                                        // 2-wciśnięty "-", 3-wciśnięty "+" i "-", 4-wciśnięty OK
    #define PARAM_MENU_SW_TIME          11  // czas przez jaki przycisk jest wciśnięty [s]
    #define PARAM_POWER_BAT_MA          12  // prąd akumulatora [mA]
    #define PARAM_POWER_BAT_V           13  // napięcie akumulatora *10 [V]
    #define PARAM_POWER_BAT_V_CALC      14  // napięcie akumulatora *10 - obliczone do poziomu naładowania [V]
    #define PARAM_HALL_MOTOR_DEG        15  // Aktualny kąt obrotu silnika
    #define PARAM_HALL_SETTER_DEG       16  // Aktualny kąt obrotu zadajnika prędkości
    #define PARAM_INIT_OK_SETTER        17  // Inicjalizacja zadajnika OK
    #define PARAM_SETTER_MIN_VALUE      18  // Minimalny kąt zadajnika
    #define PARAM_SETTER_MAX_VALUE      19  // Maksymalny kąt zadajnika
    #define PARAM_SETTER_LIM_SW_STAT    20  // 0=nie wcisniety, 2=wcisniety, -1=zbocze opadające, 1=zbocze narastające
    #define PARAM_SETTER_TACT           21  // Takty zadane

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