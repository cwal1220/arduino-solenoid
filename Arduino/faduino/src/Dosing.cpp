#include <Arduino.h>
#include "Dosing.h"

Dosing::Dosing(unsigned char _pin)
{
    pinMode(pin, OUTPUT);
    pin = _pin;
}

void Dosing::setMililiterPerMs(float _milliliterPerMs)
{
    milliliterPerMs = _milliliterPerMs;
}

void Dosing::setDoseAmount(unsigned int _doseAmount)
{
    doseAmount = _doseAmount;
    doseTime = (float)doseAmount / milliliterPerMs;
}

void Dosing::start(unsigned int _milliliter)
{
    digitalWrite(pin, HIGH);
    bgnTime = millis();
}

bool Dosing::check()
{
    endTime = millis();
    if(doseTime < (endTime - bgnTime))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Dosing::stop()
{
    digitalWrite(pin, LOW);
    bgnTime = 0;
    endTime = 0;
}
