#include "Arduino.h"
#include "Wire.h"

#define SSD1311_ADDR 0x3C

#ifndef RES
#define RES 7
#endif

class SSD1311
{
public:
	SSD1311();
	void SWRES();
	void init();
	void clear();
	void setContrast(unsigned char contrast);
	void setCursor(uint8_t, uint8_t);
	void sendCMD(unsigned char command);
	void sendData(unsigned char data);
	void sendFloat(uint8_t col, uint8_t row, float digit, uint8_t dec, uint8_t nad);
	void sendString(uint8_t col, uint8_t row, const char *String);
	void scrollString(byte row, char *message, unsigned int time);
	void dispOFF();
	void dispON();
};
