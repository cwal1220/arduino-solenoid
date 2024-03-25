#include <Arduino.h>
#include "Dosing.h"

Dosing::Dosing(unsigned char _pin)
{
    pinMode(pin, OUTPUT);
    pin = _pin;
}

void Dosing::setMililiterPerMin(unsigned int _milliliterPerMin)
{
    milliliterPerMin = _milliliterPerMin;
}

void Dosing::start(unsigned int _milliliter)
{
    digitalWrite(pin, HIGH);
    bgnTime = millis();
}

void Dosing::check()
{
    endTime = millis();
}

void Dosing::stop()
{
    digitalWrite(pin, LOW);
    bgnTime = 0;
    endTime = 0;
}
