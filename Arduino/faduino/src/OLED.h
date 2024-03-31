#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

void TCA9548A(uint8_t bus);
void drawDisplay(unsigned int oledIdx, const char *name, unsigned int dose);
void clearDisplay(unsigned int oledIdx);
void initDisplays();
void matrixPrint(int XPOS, int YPOS, const char *pChar);
byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3);

#define OLED_RESET 4