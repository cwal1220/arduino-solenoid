#ifndef DOSING_H
#define DOSING_H

class Dosing
{
public:
    Dosing();
    void initPin(unsigned char _pin);
    unsigned int getDoseAmount();
    unsigned int getDoseStat();
    void setDoseAmount(unsigned int _amount);
    void setDoseWeight(float _doseWeight);
    void upPulse();
    void downPulse();
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
    // TB6600에서 설정한 1회전당 펄스
    float pulsePerRotation = 200.0;
    float milliliterPerPulse = 0.39 * pulsePerRotation;
    unsigned int doseAmount = 0;
    float doseWeight = 1.0;
    unsigned int doseCnt = 0;
    unsigned int doseStat = STOP; // 0:WAIT 1:RUN, 2:STOP
    unsigned int prevStat = STOP; // 수동추출모드를 위해 이전 상태를 저장하기 위함.
};

#endif // DOSING_H
