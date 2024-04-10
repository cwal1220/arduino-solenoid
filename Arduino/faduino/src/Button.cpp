#include <Arduino.h>
#include "Button.h"

Button::Button()
{
}

void Button::initPin(unsigned char _buttonPin, unsigned char _ledPin)
{
    buttonPin = _buttonPin;
    ledPin = _ledPin;
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
}

bool Button::isPushed()
{
    return !digitalRead(buttonPin);
}

void Button::setLedState(unsigned char _ledState)
{
    ledState = _ledState;
    digitalWrite(ledPin, ledState);
}

unsigned char Button::getLedState()
{
    return ledState;
}

void Button::blinkStart()
{
    blinkStarted = 1;
}

void Button::blinkStop()
{
    blinkStarted = 0;
    setLedState(0);
}

unsigned char Button::getBlinkStarted()
{
    return blinkStarted;
}
