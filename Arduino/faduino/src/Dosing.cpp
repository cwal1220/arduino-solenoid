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
    doseCnt = (float)doseAmount * doseWeight * milliliterPerPulse;
    doseStat = SET;
}

void Dosing::setDoseWeight(float _doseWeight)
{
    doseWeight = _doseWeight;
}

void Dosing::upPulse()
{
    if(doseStat == RUN || doseStat == MANUAL) // RUN
    {
        digitalWrite(pin, HIGH);
    }
}

void Dosing::downPulse()
{
    if(doseStat == RUN || doseStat == MANUAL) // RUN
    {
        digitalWrite(pin, LOW);
    }
}

void Dosing::start()
{
    doseCnt = (float)doseAmount * doseWeight * milliliterPerPulse;
    doseStat = RUN; // RUN
}

void Dosing::startManual()
{
    doseStat = MANUAL;
}

unsigned int Dosing::check()
{
    if(doseStat == RUN) // RUN
    {
        doseCnt--;
        if(doseCnt <= 0)
        {
            doseStat = END; // 다 되면 END
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
    doseCnt = 0;
    doseStat = STOP;
}
