#ifndef _M_lcd_
    #define _M_lcd_
    #include <Wire.h>
    #include <LiquidCrystal_I2C.h>
    #include <arduino.h>
    #include <M_param.h>
     
    #define LCD_ADRESS_1 0x27
    #define LCD_ADRESS_2 0x3F

    typedef const struct MenuStructure
    {
        const char *text;
        int up;
        int down;
        int enter;
        int parameter;
        int factor;
        int precision;
        const char *unit;

    } MenuEntry;

    const char menu_000[] = "Zasilanie      ";
    const char menu_001[] = "Naped          ";
    const char menu_002[] = "Reg.predkosci  ";
    const char menu_003[] = "Ustawienia gl  ";
    const char menu_004[] = "Wyjscie        ";

    const char menu_005[] = "NapBat ";
    const char menu_006[] = "PrBat  ";
    const char menu_007[] = "NapObl ";

    const char menu_008[] = "KatMot ";
    const char menu_009[] = "KatZad ";
    const char menu_010[] = "InitOK ";
    const char menu_011[] = "MaxZad ";
    const char menu_012[] = "MinZad ";
    const char menu_013[] = "WylKra ";
    const char menu_014[] = "TaktZd ";

    const char menu_099[] = "Powrot         ";

    const char unit_000[] = "   ";        // space
    const char unit_001[] = "V  ";        // volt
    const char unit_002[] = "mA ";       // miliamper
    const char unit_003[] = "rpm";      // obr/min
    const char unit_004[] = "223";      // stopień - kąt

    MenuEntry _menu[] = 
    {
        {menu_000, -1,  1,  5, -1,  1, 0, unit_000},  // 0
        {menu_001,  0,  2,  9, -1,  1, 0, unit_000},  // 1
        {menu_002,  1,  3, -1, -1,  1, 0, unit_000},  // 2
        {menu_003,  2,  4, -1, -1,  1, 0, unit_000},  // 3
        {menu_004,  3, -1, -1, -1,  1, 0, unit_000},  // 4
// Zasilanie
        {menu_005, -1,  6, -1, 13, 10, 1, unit_001},  // 5
        {menu_007,  5,  7, -1, 14, 10, 1, unit_001},  // 6
        {menu_006,  6,  8, -1, 12,  1, 0, unit_002},  // 7
        {menu_099,  7, -1,  0, -1,  1, 0, unit_000},  // 8
// Napęd
        {menu_008, -1,  10, -1, 15,  10, 1, unit_000},  // 9
        {menu_009,  9,  11, -1, 16,  10, 1, unit_000},  // 10
        {menu_010,  10, 12, -1, 17,  1, 0, unit_000},  // 11
        {menu_011,  11, 13, -1, 19,  10, 1, unit_000},  // 12
        {menu_012,  12, 14, -1, 18,  10, 1, unit_000},  // 13
        {menu_013,  13, 15, -1, 20,  1, 0, unit_000},  // 14
        {menu_014,  14, 16, -1, 21,  1, 0, unit_000},  // 15
        {menu_099,  15, -1,  0, -1,  1, 0, unit_000},  // 16
    };
class M_lcd
{
private:
    uint8_t _adress = 0;
    LiquidCrystal_I2C* lcd;
    M_param* _param;
    int _intro_step = 15;
    int _status = 0; //0-intro; 1-default screen; 2-menu screen
    bool _button_pressed = false;
    int _menu_display_nr = 0;
    int _menu_val_diff_up = 0;
    int _menu_val_diff_dn = 0;
    void _intro();
    void _checkAdress(uint8_t adr);
    void _default_screen();
    void _screen_menu();
    void _menu_control();
public:
    M_lcd(M_param* param);
    void refresh();
};

#endif