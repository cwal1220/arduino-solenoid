#ifndef DOSING_H
#define DOSING_H

class Dosing
{
public:
    Dosing(unsigned char _pin);
    void setMililiterPerMs(float _milliliterPerMs);
    void setDoseAmount(unsigned int _amount);
    void start(unsigned int _milliliter);
    bool check();
    void stop();
private:
    unsigned char pin;
    float milliliterPerMs = 0.0333333;
    unsigned int doseAmount;
    unsigned int doseTime;
    unsigned long bgnTime;
    unsigned long endTime;
};

#endif // DOSING_H