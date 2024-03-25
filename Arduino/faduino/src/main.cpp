#include "OLED.h"

void setup()
{
    Serial.begin(9600);
    initDisplays();
}

void loop()
{
    for (int i = 0; i < 5; i++)
    {
        drawDisplay(i, "찬", 500);
    }
    for (int i = 0; i < 5; i++)
    {
        clearDisplay(i);
    }
    Serial.println(F("Loop..."));
}
