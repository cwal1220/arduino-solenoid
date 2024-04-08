#include <MsTimer2.h>
#include "OLED.h"
#include "Dosing.h"
#include "Button.h"
#include "FloatSensor.h"

Dosing dosingPump[5];
Button ledButton[5];
FloatSensor floatSensor[5];

String inputString;
char* DOSING_STATUS_STR[3] = {"WAIT", "RUN", "STOP"};
char* SYSTEM_STATUS_STR[2] = {"OK", "ERROR"};

void ledTimerISR();

// Total Dosing, LED Button Number
const int SENSOR_NUM = 5;

// Define Dosing Pump PINs: OUTPUT
const int DOSING_PUMP_PIN[SENSOR_NUM] = {2, 3, 4, 5, 6};

// Define Button LED PINs: OUTPUT
const int BUTTON_PUSH_PIN[SENSOR_NUM] = {30, 32, 34, 36, 38};

// Define Button Input PINs: INPUT
const int BUTTON_LED_PIN[SENSOR_NUM] = {31, 33, 35, 37, 39};

// Define Float Sensor Input PINs: INPUT
const int FLOAT_SENSOR_PIN[SENSOR_NUM] = {7, 8, 9, 10, 11};

void checkProtocol()
{
    // SET,1,홍길동,200 -> SET,1,OK,0
    // STAT,1 -> STAT,1,RUN,0
    // EXTR,1,300 -> EXTR,1,300,0(300)
    // CLEAN,1 -> CLEAN,1,OK

    // Check Protocol
    if (Serial.available())
    {
        char inChar = (char)Serial.read(); // 입력된 문자 읽기
        inputString += inChar;             // 문자열에 추가
        if (inChar == '\n')                // 사용자가 Enter를 입력했을 경우
        {
            int splitIdx1 = inputString.indexOf(",");
            String cmdStr = inputString.substring(0, splitIdx1);
            if(cmdStr.equals("SET"))
            {
                int splitIdx2 = inputString.indexOf(",", splitIdx1+1);
                int splitIdx3 = inputString.indexOf(",", splitIdx2+1);

                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, splitIdx2);
                String nameStr = inputString.substring(splitIdx2+1, splitIdx3);
                String doseAmountStr = inputString.substring(splitIdx3+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;
                unsigned int doseAmount = doseAmountStr.toInt();

                if(dosingPumpIdx < 5)
                {
                    char sendStr[40] = {'\0',};
                    unsigned char errCode;
                    if(floatSensor[dosingPumpIdx].isEmpty())
                    {
                        errCode = 1;
                    }
                    else
                    {
                        dosingPump[dosingPumpIdx].setDoseAmount(doseAmount);
                        clearDisplay(dosingPumpIdx);
                        drawDisplay(dosingPumpIdx, nameStr.c_str(), doseAmount);
                        errCode = 0;
                    }
                    sprintf(sendStr, "SET,%d,%s,%d\n", dosingPumpIdx+1, SYSTEM_STATUS_STR[errCode], errCode);
                    Serial.print(sendStr);
                }

            }
            else if(cmdStr.equals("STAT"))
            {
                int splitIdx2 = inputString.indexOf(",", splitIdx1+1);
                int splitIdx3 = inputString.indexOf(",", splitIdx2+1);

                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, splitIdx2);
                String dosingPumpStatStr = inputString.substring(splitIdx2+1, splitIdx3);
                String resultStr = inputString.substring(splitIdx3+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;

                if(dosingPumpIdx < SENSOR_NUM)
                {
                    unsigned int dosingPumpStat = dosingPump[dosingPumpIdx].getDoseStat();
                    char sendStr[40] = {'\0',};
                    sprintf(sendStr, "STAT,%d,%s,0\n", dosingPumpIdx+1, DOSING_STATUS_STR[dosingPumpStat]);
                    Serial.print(sendStr);
                }
            }
            else if(cmdStr.equals("EXTR"))
            {
                int splitIdx2 = inputString.indexOf(",", splitIdx1+1);

                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, splitIdx2);
                String doseAmountStr = inputString.substring(splitIdx2+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;
                unsigned int doseAmount = doseAmountStr.toInt();

                if(dosingPumpIdx < SENSOR_NUM)
                {
                    dosingPump[dosingPumpIdx].setDoseAmount(doseAmount);
                    ledButton[dosingPumpIdx].blinkStart();
                }
            }
            else if(cmdStr.equals("CLEAN"))
            {
                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;

                if(dosingPumpIdx < SENSOR_NUM)
                {
                    dosingPump[dosingPumpIdx].setDoseAmount(200);
                    clearDisplay(dosingPumpIdx);
                    drawDisplay(dosingPumpIdx, "CLEAN", 200);
                }
            }
            inputString = ""; // 입력 문자열 초기화
        }
    }
}

void checkButton()
{
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(dosingPump[idx].getDoseStat() == Dosing::WAIT && ledButton[idx].isPushed())
        {
            dosingPump[idx].start();
            char sendStr[40] = {'\0',};
            sprintf(sendStr, "EXTR,%d,0,%d\n", idx+1, dosingPump[idx].getDoseAmount());
            Serial.print(sendStr);
            ledButton[idx].blinkStop();
        }
    }
}

void checkDosingPump()
{
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(dosingPump[idx].check() == Dosing::END)
        {
            dosingPump[idx].stop();
            char sendStr[40] = {'\0',};
            sprintf(sendStr, "EXTR,%d,%d,%d\n", idx+1, dosingPump[idx].getDoseAmount(), dosingPump[idx].getDoseAmount());
            Serial.print(sendStr);
        }
    }
}

void ledTimerISR()
{
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(ledButton[idx].getBlinkStarted())
        {
            ledButton[idx].setLedState(!ledButton[idx].getLedState());
        }
    }
}

void setup()
{
    Serial.begin(9600);
    initDisplays();
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        dosingPump[idx].initPin(DOSING_PUMP_PIN[idx]);
        ledButton[idx].initPin(BUTTON_PUSH_PIN[idx], BUTTON_LED_PIN[idx]);
        floatSensor[idx].initPin(FLOAT_SENSOR_PIN[idx]);
    }
    MsTimer2::set(1000, ledTimerISR);
    MsTimer2::start();
}

void loop()
{
    // Check Protocol
    checkProtocol();

    // Check Button Status
    checkButton();

    // Check Dosing Pump
    checkDosingPump();

    // TODO: Check Float Sensor Status
}
