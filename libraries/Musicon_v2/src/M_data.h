#pragma once
#ifndef _M_data_
    #define _M_data_

    #include <EEPROM.h>

    #define PARAM_RESTORE               0   // 1001 - parametry zostały przywrócone, każda inna wartość - przywrócenie wartości domyślnych po następnym uruchomieniu
    #define PARAM_POWER_STATUS          1   // 0 - wyłączony, 1 - uśpiony i ładuje, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
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
    #define PARAM_I2C_STATUS            22  // Status szyny I2C - 0-błąd magistrali I2C, 1-OK
    #define PARAM_MOTOR_PERM_TO_MOVE    23  // Status załączenia silnika - 0 - stop, 1 - ruch, 2 - hamowanie
    #define PARAM_MOTOR_TMC_SG_RESULT   24  // TMC - SG_RESULT 
    #define PARAM_MOTOR_TMC_CS_ACTUAL   25  // TMC - CS_ACTUAL 
    #define PARAM_MOTOR_TMC_TSTEP       26  // TMC - TSTEP  
    #define PARAM_MOTOR_TMC_VELOCITY    27  // wartość okreslająca prędkość silnika = 1 RPM
    #define PARAM_MOTOR_TMC_MIN_VEL     28  // minimalne obroty bębna (x10)
    #define PARAM_MOTOR_TMC_MAX_VEL     29  // maksymale obroty bębna (x10)
    #define PARAM_MOTOR_RATIO           30  // przełożenie bęben-silnik 1:x (x10)
    #define PARAM_MOTOR_VELOCITY        31  // prędkość silnika (RPM x 10)
    
    #define PARAM_MOTOR_BRAKE_VEL       32  // prędkość przy której rozpoczyna się hamowanie (RPM x 10)
    #define PARAM_MOTOR_BRAKE_TIME      33  // czas przez który musi być przekroczona prędkość, by zacząć hamować [ms]
    #define PARAM_POWER_BAT_MIN         34  // Napięcie minimalne baterii [V x10]
    #define PARAM_POWER_BAT_STAT        35  // Status baterii: -1-brak baterii, 0-100-Procent naładowania
    #define PARAM_POWER_SUPPLY_OK       36  // 0-zasilacz nie podłączony, 1-zasilacz podłączony
    #define PARAM_POWER_BAT_A_MAX       37  // maksymalny prąd ładujący baterię [mA]
    #define PARAM_POWER_BAT_PWM         38  // PWM ładowania
    #define PARAM_POWER_BAT_CHARGE      39  // 0-brak ładowania, 1-ładowanie
    #define PARAM_MPLX_TYPE             40  // 43-TCA9543A, 42-PI4MSD5V9542A
    #define PARAM_INACTION_TIME         41  // czas bezczynności
    #define PARAM_INACTION_TIME_OFF     42  // czas bezczynności po którym następuje wyłączenie
    #define PARAM_TEST                  43  // parametr do testów
    #define PARAM_TACT_MAX_VALUE        44  // Takty maksymalne
    #define PARAM_TACT_MIN_VALUE        45  // Takty minimalne

    class M_data
    {
    private:
        int _parameters[200] = {0};
        void _writeParamToEEPROM(int param_nr, int val);
        void _readParamToEEPROM(int param_nr);
        void _init();
    public:
        M_data();
        bool init_ok = false;
        int parameter(int param_nr);
        int parameter(int param_nr, int change_values, bool save_to_eeprom = false);
        void restore();
    };

#endif