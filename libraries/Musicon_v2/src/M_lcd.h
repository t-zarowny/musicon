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
        const char *unit;

    } MenuEntry;

    const char menu_000[] = "Zasilanie    ";
    const char menu_001[] = "Naped        ";
    const char menu_002[] = "Reg.predkosci";
    const char menu_003[] = "Ustawienia gl";
    const char menu_004[] = "Wyjscie      ";

    const char unit_000[] = " ";        // space
    const char unit_001[] = "V";        // volt
    const char unit_002[] = "mA";       // miliamper
    const char unit_003[] = "rpm";      // obr/min
    const char unit_004[] = "223";      // stopień - kąt

    MenuEntry _menu[] = 
    {
        {menu_000, -1, 1, -1, 0, 0, unit_000},     // 0
        {menu_001, 0, 2, -1, 0, 0, unit_000},      // 1
        {menu_002, 1, 3, -1, 0, 0, unit_000},      // 2
        {menu_003, 2, 4, -1, 0, 0, unit_000},      // 3
        {menu_004, 3, -1, -1, 0, 0, unit_000},     // 4
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
    void _intro();
    void _checkAdress(uint8_t adr);
    void _default_screen();
    void _screen_menu();
    void _menu_control();
public:
    M_lcd(M_param* param);
    void init();
    void refresh();
};

#endif