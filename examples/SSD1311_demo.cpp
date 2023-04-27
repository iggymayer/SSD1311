#include "SSD1311.h"

SSD1311 oled;

void setup()
{
    oled.init();
    oled.setContrast(10);
    oled.sendString(0, 0, "Hello World!");
}

void loop()
{
    oled.dispON();
    delay(1000);
    oled.dispOFF();
    delay(1000);
}
