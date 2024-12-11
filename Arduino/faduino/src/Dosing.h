#ifndef DOSING_H
#define DOSING_H

class Dosing
{
public:
    Dosing();
    void initPin(unsigned char _pin);
    void setMililiterPerMs(float _milliliterPerMs);
    unsigned int getDoseAmount();
    unsigned int getDoseStat();
    void setDoseAmount(unsigned int _amount);
    void setDoseWeight(float _doseWeight);
    void start();
    void startManual();
    unsigned int check();
    void wait();
    void stop();

    static const unsigned int STOP = 0;
    static const unsigned int SET = 1;
    static const unsigned int WAIT = 2;
    static const unsigned int RUN = 3;
    static const unsigned int END = 4;
    static const unsigned int MANUAL = 5;
private:
    unsigned char pin;
    float milliliterPerMs = 0.016566633;
    unsigned int doseAmount = 0;
    float doseWeight = 1.0;
    unsigned int doseTime = 0;
    unsigned long bgnTime = 0;
    unsigned long endTime = 0;
    unsigned int doseStat = STOP; // 0:WAIT 1:RUN, 2:STOP
};

#endif // DOSING_H