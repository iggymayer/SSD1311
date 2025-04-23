#include "SSD1311.h"

SSD1311 oled(0x3C, 4, 16, 2);

void setup()
{
    oled.init();
    oled.setContrast(127);
    oled.sendString(0, 0, "Hello World!");
}

void loop()
{
    oled.dispON();
    delay(1000);
    oled.dispOFF();
    delay(1000);
}
