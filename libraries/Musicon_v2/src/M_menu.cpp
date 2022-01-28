#ifndef _MenuStructure_
#define _MenuStructure_
    typedef const struct MenuStructure
    {
        int parameter;
        int factor;
        int precision;
        bool editable;
    } MenuEntry;

    MenuEntry _menu[] = 
    {
        //Bateria
        {12,     1,     0,  0},  // prąd akumulatora [mA]
        {13,    10,     1,  0},  // napięcie akumulatora *10 [V]
        {14,    10,     1,  0},  // napięcie akumulatora *10 - obliczone do poziomu naładowania [V]
        {35,     1,     0,  0},  // Status baterii: -1-brak baterii, 0-100-Procent naładowania
        {38,     1,     0,  0},  // PWM ładowania
        {39,     1,     0,  0},  // 0-brak ładowania, 1-ładowanie

        //Silnik
        {15,    10,     1,  0},  // Aktualny kąt obrotu silnika
        {27,     1,     0,  1},  // wartość okreslająca prędkość silnika = 1 RPM
        {28,    10,     1,  0},  // minimalne obroty bębna (x10)
        {29,    10,     1,  0},  // maksymale obroty bębna (x10)
        {30,    10,     1,  0},  // przełożenie bęben-silnik 1:x (x10)
        {31,    10,     1,  0},  // prędkość silnika (RPM x 10)
      
        //Zadajnik
        {16,    10,     1,  0},  // Aktualny kąt obrotu zadajnika prędkości
        {18,    10,     1,  0},  // Minimalny kąt zadajnika
        {19,    10,     1,  0},  // Maksymalny kąt zadajnika
        {21,     1,     0,  0},  // Takty zadane
        {27,     1,     0,  1},  // wartość okreslająca prędkość silnika = 1 RPM
    
        //ogólne        
        {0,      1,     1,  1},  // 1001 - parametry zostały przywrócone, każda inna wartość - przywrócenie wartości domyślnych po następnym uruchomieniu
        {11,     1,     1,  0},  // czas przez jaki przycisk jest wciśnięty [s]
        {40,     1,     0,  0},  // 43-TCA9543A, 42-PI4MSD5V9542A
        {41,     1,     0,  0},  // czas bezczynności
        {42,     1,     0,  1},  // czas bezczynności po którym następuje wyłączenie
    };

#endif