#ifndef FLOATSENSOR_H
#define FLOATSENSOR_H

class FloatSensor
{
public:
    FloatSensor();
    void initPin(unsigned char _floatSensorPin);
    unsigned char isEmpty();

private:
    unsigned char floatSensorPin;
};

#endif // FLOATSENSOR_H