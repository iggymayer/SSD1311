#include "Wire.h"
#include "SSD1311.h"

SSD1311::SSD1311(uint8_t addr, uint8_t res, uint8_t cols, uint8_t rows) : address(addr), resetPin(res), numCols(cols), numRows(rows)
{}

void SSD1311::init()
{
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  delayMicroseconds(5);
  digitalWrite(resetPin, HIGH);
  delayMicroseconds(100);

  // Set Internal Regulator
  sendCMD(0x2A); // Set RE=1	00101010B
  sendCMD(0x71); // Internal Regulator Mode
  // sendCMD(0x00); // Internal Regulator OFF 3.3V-INPUT
  sendCMD(0x5C); // Internal Regulator ON 5V-INPUT
  sendCMD(0x28); // Set IS=0 ,RE =0

  // Logic OFF
  sendCMD(DISPLAY_OFF); // Display OFF

  // Initial Settings Configuration
  sendCMD(0x2A); // Set RE=1	00101010B
  sendCMD(0x79); // Set SD=1	01111001B/CMD Set Enabled

  // Set Display Clock Divide Ratio/Oscillator Frequency
  sendCMD(0xD5);
  sendCMD(0x70);

  // CMD Set Disabled
  sendCMD(0x78); // Set SD=0  01111000B

  // Assign font width,black/white inverting of cursor,and 4-line display mode control bit.
  // Binary 00001ABC
  // Define A:Assign Font Width,0 = 5-dot Font Width,1 = 6-dot Font Width.
  // Define B:Black/White Inverting of Cursor,0 = Inverting Disabled,1 = Inverting Enabled.
  // Define C:4-line Display Mode Control Bit,0 = 1 or 2 Line,1 = 3 or 4 Line.
  sendCMD(0x08); // 00001000:5-dot Font Width,Inverting Disabled,1 or 2 line.Default
  // sendCMD(0x09); // 00001001:5-dot Font Width,Inverting Disabled,3 or 4 line.
  // sendCMD(0x0A); // 00001010:5-dot Font Width,Inverting Enabled,1 or 2 line.
  // sendCMD(0x0B); // 00001011:5-dot Font Width,Inverting Enabled,3 or 4 line.

  // Common Bi-direction Function/SHLC/SHLS
  // sendCMD(0x04); // 00000100:Set COM 31->00,SEG 99->00.
  // sendCMD(0x05); // 00000101:Set COM 31->00,SEG 00->99.
  // sendCMD(0x06); // 00000110:Set COM 00->31,SEG 99->00.
  sendCMD(0x07); // 00000111:Set COM 00->31,SEG 00->99.Default

  //  CGROM/CGRAM Management/ROM/OPR
  sendCMD(0x72); // 01110010
  sendCMD(0x00); // ****0000:Set ROM A & 8 CGRAM
  // sendCMD(0x0A); // ****1010:Set ROM C & 6 CGRAM

  // Set OLED Characterization
  sendCMD(0x2A); // Set RE=1
  sendCMD(0x79); // Set SD=1/CMD Set Enabled

  // Set SEG Pins Hardware Configuration
  // Binary 00AB 0000
  // Define A:0 = Disable SEG Left/Right remap,1 = Enable SEG Left/Right remap.
  // Define B:0 = Sequential SEG pin configuration,1 = Alternative (odd/even) SEG pin configuration.
  sendCMD(0xDA); // Set SEG Pins Hardware Configuration
  sendCMD(0x10); // 0001 0000 Default
  // sendCMD(0x20); // 0010 0000
  // sendCMD(0x30); // 0011 0000

  // Set Segment Low Voltage & GPIO
  // Binary A000 00BC
  // Define A:Set VSL,0 = Internal VSL,1 = External VSL.
  // Define BC:Set GPIO,00 = GPIO HiZ,Input Disabled;01 = GPIO HiZ,Input Enabled;10 = GPIO Output LOW;11 = GPIO Output HIGH.
  sendCMD(0xDC); // 11011100
  sendCMD(0x03); // 00000011:Internal VSL,GPIO Output HIGH.
  // sendCMD(0x02); // 00000010:Internal VSL,GPIO Output LOW.

  // Set Contrast
  sendCMD(DISPLAY_CONTRAST); // 10000001
  sendCMD(0xFF); // DEC:255

  // Set Pre-charge Period
  sendCMD(0xD9); // 11011001:Set Phase Length
  sendCMD(0xF1); // 11110001

  // Set VCOMH Deselect Level
  sendCMD(0xDB); // 11011011
  sendCMD(0x20); // 00100000:VCC*0.77
  // sendCMD(0x30); // 00110000:VCC*0.83

  // CMD Mode Disabled
  sendCMD(0x78); // CMD Set Disabled
  sendCMD(0x28); // Set IS=0 ,RE =0

  sendCMD(0x2A); // Set RE=1
  sendCMD(0x06); // Set Entry Mode
  sendCMD(0x08); // Logic OFF
  sendCMD(0x28); // Set IS=0 ,RE =0

  // Clear Display
  sendCMD(DISPLAY_CLEAR);

  // Set DDRAM Address
  sendCMD(0x80); // 10000000.Default

  // Logic ON
  sendCMD(DISPLAY_ON); // Display ON

  delay(200);
}

void SSD1311::clear()
{
  sendCMD(DISPLAY_CLEAR);
}

void SSD1311::setContrast(unsigned char contrast)
{
  // CMD Set Enabled
  sendCMD(0x2A); // Set RE=1	00101010B
  sendCMD(0x79); // Set SD=1	01111001B

  sendCMD(DISPLAY_CONTRAST);     // Set Contrast
  sendCMD(contrast); // send contrast value
  sendCMD(0x78);     // CMD Set Disabled
  sendCMD(0x28);
}

void SSD1311::setCursor(uint8_t col, uint8_t row)
{
  if (row > numRows || col > numCols)
    return;
  int row_offsets[] = {0x00, 0x40};
  sendCMD(0x80 | (col + row_offsets[row]));
}

void SSD1311::sendCMD(unsigned char command)
{
  Wire.beginTransmission(address);
  Wire.write(0X80); // Set Command mode
  Wire.write(command);
  Wire.endTransmission();
  delayMicroseconds(100);
}

void SSD1311::sendData(unsigned char data)
{
  Wire.beginTransmission(address);
  Wire.write(0x40); //  Set Data mode
  Wire.write(data);
  Wire.endTransmission();
  delayMicroseconds(100);
}

void SSD1311::sendFloat(uint8_t col, uint8_t row, float digit, uint8_t dec, uint8_t nad)
{
  char line[17];                  // 16 characters + null terminator
  dtostrf(digit, dec, nad, line); // Convert the float value to a string
  line[16] = '\0';                // Ensure null termination
  sendStr(col, row, line);
}

void SSD1311::sendStr(uint8_t col, uint8_t row, const char *String)
{
  setCursor(col, row);
  unsigned char i = 0;
  while (String[i])
  {
    sendData(String[i]);
    i++;
  }
}

void SSD1311::scrollStr(byte row, const char *message, unsigned int speed, uint8_t displayWidth)
{
  if (displayWidth > 16) displayWidth = 16;
  char *buffer = new char[displayWidth + 1]();
  size_t messageLen = strlen(message);

  for (size_t i = 0; i < messageLen + displayWidth; i++)
  {
    for (size_t j = 0; j < displayWidth; j++)
    {
      size_t pos = i + j - displayWidth;
      buffer[j] = (pos < messageLen) ? message[pos] : ' ';
    }
    buffer[displayWidth] = '\0';

    sendStr(0, row, buffer);
    delay(speed);
  }

  delete[] buffer;
}

void SSD1311::dispOFF()
{
  // Logic Off
  sendCMD(DISPLAY_OFF); // Display OFF
  delay(100);

  // HV OFF
  sendCMD(0x2A); // Set RE=1	00101010B
  sendCMD(0x79); // Set SD=1	01111001B/Command Set Enabled
  sendCMD(0xDC); // 11011100:Set VSL & GPIO
  sendCMD(0x02); // Internal VSL,GPIO-OUTPUT(HV) LOW.
  sendCMD(0x78); // Command Set Disabled
  sendCMD(0x28); // IS=0 ,RE=0
}

void SSD1311::dispON()
{
  // HV ON
  sendCMD(0x2A); // Set RE=1	00101010B
  sendCMD(0x79); // Set SD=1	01111001B/Command Set Enabled
  sendCMD(0xDC); // 11011100:Set VSL & GPIO
  sendCMD(0x03); // Internal VSL,GPIO-OUTPUT(HV) HIGH.
  sendCMD(0x78); // Command Set Disabled
  sendCMD(0x28); // Set IS=0 ,RE=0
  delay(100);

  // Logic ON
  sendCMD(DISPLAY_ON); // Display ON
}
