#include <Arduino.h>
#include "Dosing.h"

Dosing::Dosing()
{
}

void Dosing::initPin(unsigned char _pin)
{
    pin = _pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
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

void Dosing::setDoseWeight(float _doseWeight)
{
    doseWeight = _doseWeight;
}

void Dosing::start()
{
    digitalWrite(pin, LOW);
    doseTime = ((float)doseAmount * doseWeight) / milliliterPerMs;
    bgnTime = millis();
    doseStat = RUN; // RUN
}

void Dosing::startManual()
{
    digitalWrite(pin, LOW);
    doseStat = MANUAL;
}


unsigned int Dosing::check()
{
    if(doseStat == RUN) // RUN
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
    digitalWrite(pin, HIGH);
    bgnTime = 0;
    endTime = 0;
    doseTime = 0;
    doseStat = STOP;
}
