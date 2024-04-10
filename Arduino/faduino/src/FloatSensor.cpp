#include <Arduino.h>
#include "FloatSensor.h"

FloatSensor::FloatSensor()
{
}

void FloatSensor::initPin(unsigned char _floatSensorPin)
{
    floatSensorPin = _floatSensorPin;
    pinMode(floatSensorPin, INPUT_PULLUP);
}

unsigned char FloatSensor::isEmpty()
{
    // TODO: reverse
    return digitalRead(floatSensorPin);
}

