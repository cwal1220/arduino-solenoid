#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SSD1306_NO_SPLASH // Disable Adafruit Bitmap
//Adafruit_SSD1306.h에서 # define SSD1306_128_32 주석 및 #define SSD1306_128_64 주석 해제

void TCA9548A(uint8_t bus);
void drawDisplay(unsigned int oledIdx, const char *name, unsigned int dose);
void drawWait(unsigned int oledIdx);
void drawExtr(unsigned int oledIdx);
void drawDone(unsigned int oledIdx);
void clearDisplay(unsigned int oledIdx);
void initDisplays();
void matrixPrint(int XPOS, int YPOS, const char *pChar);
byte *getHAN_font(byte HAN1, byte HAN2, byte HAN3);

#define OLED_RESET 4