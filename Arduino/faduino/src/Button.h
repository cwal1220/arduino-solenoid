#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:
    Button();
    void initPin(unsigned char _buttonPin, unsigned char _ledPin);
    bool isPushed();
    void setLedState(unsigned char ledState);
    unsigned char getLedState();
    void blinkStart();
    void blinkStop();
    unsigned char getBlinkStarted();

private:
    unsigned char buttonPin;
    unsigned char ledPin;
    unsigned char ledState = 0;
    unsigned char blinkStarted = 0;
};

#endif // BUTTON_H