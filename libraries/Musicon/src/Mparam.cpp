#include "Mparam.h"

Mparam::Mparam(){
        for(int i=0; i<80; i++){
                param[i] = 0;
        }
        //nazwa = {"M","u","s","i","c","o","n"};
        //nazwa = "Musicon";
        //wersja = "v3.0.0";
        //setNazwa("Musicon");
        init(0, 0);             // 0 - dolna wartosc potencjometru
        init(1, 0);             // 1 - gorna wartosc potencjometru
        init(2, 260);           // 2 - wartosc potencjometru dzielaca wylaczniki krancowe
        init(3, 60);            // 3 - przełożenie bęben-silnik 1:x (x10)
        init(4, 32);            // 4 - ilość wierszy przycisków na obwodzie bębna
        init(5, 10);            // 5 - minimalne obroty bębna (x10)
        init(6, 45);            // 6 - maksymale obroty bębna (x10)
        init(7, 1770);          // 7 - wewnętrzna wartość okreslająca prędkość silnika = 1 RPM
        init(8, 8);             // 8 - ilość magnesów na obwodzie koła
        init(9, 8000);          // 9 - maksymalny czas oczekiwania na sygnał z czujnika [ms]
        init(10, 5072);         // 10 - Wartość wejścia analogowego przy prądzie 0 (x10)
        init(11, 276);          // 11 - Ilość mA na jeden punkt wartości wejścia analogowego (x10)
        init(12, 510);          // 12 - punkt ADC przycisku menu "-"
        init(13, 320);          // 13 - punkt ADC przycisku menu "+"
        init(14, 170);          // 14 - punkt ADC przycisku menu "OK"
        init(15, 50);           // 15 - tolerancja +- od punktu menu ADC
        init(16, 400);          // 16 - interwał czasowy odświeżania LCD [ms]
        init(17, 2000);         // 17 - czas wyświetlania komunikatu wyłączenia [ms]
        init(18, 300);          // 18 - czas bezczynności po którym sam się wyłączy [s]
        init(19, 2500);         // 19 - napięcie czujnika prądu, przy prądzie równym 0 (było 2472)
        init(20, 600);          // 20 - maksymalny prąd ładowania [mA]
        init(21, 3500);         // 21 - interwał czasowy odświeżania PWM ładowania baterii [ms]
        init(22, 65);           // 22 - PWM dla wyłączonego ładowania
        init(23, 80);           // 23 - PWM maksymalne ładowanie
        init(24, 1);            // 24 - numer seryjny
                                // 25 - aktualna wartosc potencjometru
                                // 26 - promil prędkości zadanej
                                // 27 i 28 - takty zadane x100
                                // 29 i 30 - rpm zadane x100
                                // 31 i 32 - tmc VMAX
                                // 33 - zezwolenie na ruch
                                // 34 - zatrzymaj silnik
                                // 35 - TMC enable
                                // 36 - dolny wyłącznik krańcowy naciśnięty
                                // 37 - górny wyłącznik krańcowy naciśnięty
                                // 38 - kontrola z programu serwisowego
                                // 39 - aktualna wartosc potencjometru - program serwisowy
                                // 40 - czas pomiędzy odczytami z czujnika
                                // 41 - obliczona prędkość bębna (RPM x100)
                                // 42 - odczyt z pomiaru napięcia baterii - smooth
                                // 43 - odczyt z czujnika prądu
                                // 44 - odczyt z czujnika prądu [mA] 
                                // 45 - odczyt napięcia baterii (x10)
                                // 46 - wartość wejścia przycisków menu
                                // 47 - takty zadane
                                // 48 - nr menu
                                // 49 - menu - tryb edycji parametru
                                // 50 - menu - nr parametru do edycji
                                // 51 - menu - pamięć ostatniego przycisku
                                // 52 - tryb pracy (0 - wyłączony, 1 - ładowanie, 2 - wyłączanie ładowanie zakończone, 3 - włączony, 
                                //                      4 - wyłączanie, 5 - wyłączanie ze względu na słabą baterię)
                                // 53 - procent naładowania baterii
                                // 54 - napięcie wyjścia czujnika prądu
                                // 55 - PWM dla ładowania baterii
                                // 56 - Rodzaj zasilania - 1-bateria, 2-zasilacz
                                // 57 - Ładowanie baterii - 1-ładowanie w trakcie, 3 - ładowanie zakończone
                                // 58 - PWM dla wyłączonego ładowania
                                // 59 - 1 - działanie, 0 - bezczynność       
                                // 60 - TMC - SG_RESULT         
                                // 61 - TMC - CS_ACTUAL      
                                // 62 - TMC - TSTEP      
                                // 63 - obliczona maksymalna wartość obciążenia w czasie 1s
                                // 64 - liczenie odczytów 0% baterii
                                // 65 - różnica odczytu potencjometru akutalnego i ostatniego 
                                // 66 - dolny wyłącznik krańcowy naciśnięty - zbocze narastające 

        init(70, 100);          // 70 - Wartość prądu przy naładowaniu baterii 100% [mA]
        init(71, 127);          // 71 - Napięcie minimalne baterii [V x10]
        init(72, 164);          // 72 - Napięcie maksymalne baterii [V x10]
        init(73, 5);            // 73 - Spadek napięcia przy wyłączonym zasilaniu [V x10]
        init(74, 10);           // 74 - Spadek napięcia przy wyłączonym zasilaniu i włączonym napędzie [V x10]
        init(75, -15);          // 75 - wartość prądu poniżej której uznane jest zasilanie z baterii a wyżej z zasilacza [mA]
        init(76, 900);          // 76 - prędkość RPM, powyżej której należy hamować
        init(77, 1);            // 77 - czas hamowania
        init(78, 4);            // 78 - czas do zatrzymania podczas maksymalnego obciążenia
        init(79, 3);            // 79 - ilość odczytów 0% baterii do wyłączenia (co czas w par 21)
}
void Mparam::init(int nrParam, int def){
        param[nrParam] = read(nrParam);
        if(param[nrParam] < 0){ param[nrParam] = def; }  
}
int Mparam::get(int nrParam){
        return param[nrParam];
}
void Mparam::set(int nrParam, int valParam){
        if(nrParam > 25 && nrParam < 70){
            param[nrParam] = valParam;    
        }else{
            save(nrParam, valParam);
            param[nrParam] = valParam;    
        }
}

void Mparam::save(int nrParam, int val){
        int val_0 = 0;
        int val_1 = 1;
        if(val > 255){
            val_0 = val/255;
            val_1 = val % 255;
        }else{
            val_0 = 0;
            val_1 = val;
        }
        int adr_1 = (nrParam*2) + 1;
        int adr_2 = adr_1 + 1;
        int adr_3 = adr_1 + 100;
        EEPROM.write(adr_1, val_0);
        EEPROM.write(adr_2, val_1);
        EEPROM.write(adr_3, 1);
}
int Mparam::read(int nrParam){
        int adr_1 = (nrParam*2) + 1;
        int adr_2 = adr_1 + 1; 
        int adr_3 = adr_1 + 100;
        
        int val_0 = EEPROM.read(adr_1);
        int val_1 = EEPROM.read(adr_2);  
        int val_3 = EEPROM.read(adr_3);
        if(val_3 == 1){
            int val = (val_0*255) + val_1;
            return val;         
        }else{
            return -1;  
        }
}