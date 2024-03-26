#ifndef DOSING_H
#define DOSING_H

class Dosing
{
public:
    Dosing(unsigned char _pin);
    void setMililiterPerMin(unsigned int _milliliterPerMin);
    void start(unsigned int _milliliter);
    void check();
    void stop();
private:
    unsigned char pin;
    unsigned int milliliterPerMin = 2000;
    unsigned long bgnTime;
    unsigned long endTime;
};

#endif // DOSING_H