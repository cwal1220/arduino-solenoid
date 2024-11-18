#include <Arduino.h>
#include "Dosing.h"

Dosing::Dosing()
{
}

void Dosing::initPin(unsigned char _pin)
{
    pin = _pin;
    pinMode(pin, OUTPUT);
}

void Dosing::setMililiterPerMs(float _milliliterPerMs)
{
    milliliterPerMs = _milliliterPerMs;
}

unsigned int Dosing::getDoseAmount()
{
    return doseAmount;
}

unsigned int Dosing::getDoseStat()
{
    return doseStat;
}

void Dosing::setDoseAmount(unsigned int _doseAmount)
{
    doseAmount = _doseAmount;
    doseTime = (float)doseAmount / milliliterPerMs;
    doseStat = SET;
}

void Dosing::start()
{
    digitalWrite(pin, HIGH);
    bgnTime = millis();
    doseStat = RUN; // RUN
}

void Dosing::startManual()
{
    digitalWrite(pin, HIGH);
    doseStat = MANUAL;
}


unsigned int Dosing::check()
{
    if(doseStat == 1) // RUN
    {
        endTime = millis();
        if(doseTime < (endTime - bgnTime))
        {
            doseStat = END; // 시간이 다 되면 END
        }
        else
        {
            doseStat = RUN; // 아직일 경우 RUN
        }
    }

    return doseStat;
}

void Dosing::wait()
{
    doseStat = WAIT;
}

void Dosing::stop()
{
    digitalWrite(pin, LOW);
    bgnTime = 0;
    endTime = 0;
    doseTime = 0;
    doseStat = STOP;
}
