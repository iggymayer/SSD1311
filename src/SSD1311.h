#include "Arduino.h"
#include "Wire.h"

#define DISPLAY_CLEAR 		0x01
#define DISPLAY_ON 			0x0C
#define DISPLAY_OFF 		0x08
#define DISPLAY_CONTRAST 	0x81

// 0x3C / 0x3D

class SSD1311
{
	public:
		SSD1311(uint8_t addr, uint8_t res, uint8_t cols = 16, uint8_t rows = 2);
		void init();
		void clear();
		void setContrast(unsigned char contrast);
		void setCursor(uint8_t, uint8_t);
		void sendCMD(unsigned char command);
		void sendData(unsigned char data);
		void sendFloat(uint8_t col, uint8_t row, float digit, uint8_t dec, uint8_t nad);
		void sendStr(uint8_t col, uint8_t row, const char *String);
		void scrollStr(byte row, const char *message, unsigned int speed, uint8_t displayWidth = 16);
		void dispOFF();
		void dispON();

	private:
		uint8_t address;
		uint8_t resetPin;
		uint8_t numCols;
		uint8_t numRows;
};
