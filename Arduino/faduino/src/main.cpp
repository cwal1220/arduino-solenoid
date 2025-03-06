#include <MsTimer2.h>
#include "OLED.h"
#include "Dosing.h"
#include "Button.h"
#include <avr/wdt.h>

Dosing dosingPump[5];
Button ledButton[5];

int8_t Timer_Flag = 0;
int8_t Timer_Toggle_Flag = 0;
const int8_t AliveLedPin = 51;
int8_t Dosing_Extr_Cmd[5];

String inputString;
char* DOSING_STATUS_STR[3] = {"WAIT", "RUN", "STOP"};
char* SYSTEM_STATUS_STR[2] = {"OK", "ERROR"};

void ledTimerISR();

// Total Dosing, LED Button Number
const int SENSOR_NUM = 5;

// Define Dosing Pump PINs: OUTPUT
// const int DOSING_PUMP_PIN[SENSOR_NUM] = {2, 3, 4, 5, 6}; // Arduino Mega
const int DOSING_PUMP_PIN[SENSOR_NUM] = {6, 7, 8, 9, 10}; // Faduino

// Define Button LED PINs: OUTPUT
// const int BUTTON_LED_PIN[SENSOR_NUM] = {31, 33, 35, 37, 39}; // Arduino Mega
const int BUTTON_LED_PIN[SENSOR_NUM] = {38, 39, 40, 41, 42}; // Faduino

// Define Button Input PINs: INPUT
// const int BUTTON_PUSH_PIN[SENSOR_NUM] = {30, 32, 34, 36, 38}; // Arduino Mega
const int BUTTON_PUSH_PIN[SENSOR_NUM] = {37, 36, 35, 34, 33}; // Faduino

// Define Emergency Input PINs: INPUT
const int BUTTON_EMERGENCY_PIN[SENSOR_NUM] = {26, 25, 24, 23, 22};

// Define Manual Input PINs: INPUT
// const int BUTTON_MANUAL_PIN[SENSOR_NUM] = {7, 8, 9, 10, 11}; // Arduino Mega
const int BUTTON_MANUAL_PIN[SENSOR_NUM] = {32, 31, 30, 29, 28}; // Faduino

const int DOSING_PUMP_ENABLE_PIN[SENSOR_NUM] = {43, 44, 45, 46, 47};

void checkProtocol()
{
    // SET,1,홍길동,타이레놀,200 -> SET,1,OK,0
    // STAT,1 -> STAT,1,RUN,0
    // EXTR,1,300 -> EXTR,1,300,0(300)
    // CLEAN,1 -> CLEAN,1,OK

    // Check Protocol
    if (Serial.available())
    {
        char inChar = (char)Serial.read(); // 입력된 문자 읽기 check return VALUE TYPE
        inputString += inChar;             // 문자열에 추가
        if (inChar == '\n')                // 사용자가 Enter를 입력했을 경우
        {
            int splitIdx1 = inputString.indexOf(",");
            String cmdStr = inputString.substring(0, splitIdx1); //STRING은 문자열을 다루는 클래스, cmdstr-command string
            if(cmdStr.equals("SET"))
            {
                int splitIdx2 = inputString.indexOf(",", splitIdx1+1);
                int splitIdx3 = inputString.indexOf(",", splitIdx2+1);
                int splitIdx4 = inputString.indexOf(",", splitIdx3+1);
                int splitIdx5 = inputString.indexOf(",", splitIdx4+1);

                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, splitIdx2);
                String userNameStr = inputString.substring(splitIdx2+1, splitIdx3);
                String drugNameStr = inputString.substring(splitIdx3+1, splitIdx4);
                String doseAmountStr = inputString.substring(splitIdx4+1, splitIdx5);
                String doseWeightStr = inputString.substring(splitIdx5+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;
                unsigned int doseAmount = doseAmountStr.toInt();
                float doseWeight = doseWeightStr.toFloat();

                if(dosingPumpIdx < 5)
                {
                    char sendStr[40] = {'\0',};
                    unsigned char errCode;
                    dosingPump[dosingPumpIdx].setDoseAmount(doseAmount);
                    dosingPump[dosingPumpIdx].setDoseWeight(doseWeight);
                    clearDisplay(dosingPumpIdx);
                    drawDisplay(dosingPumpIdx, userNameStr.c_str(), drugNameStr.c_str(), doseAmount);
                    errCode = 0;
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
                    sprintf(sendStr, "STAT,%d,%s,%d\n", dosingPumpIdx+1, DOSING_STATUS_STR[dosingPumpStat], Dosing_Extr_Cmd[dosingPumpIdx]);
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
                    dosingPump[dosingPumpIdx].wait();
                    ledButton[dosingPumpIdx].blinkStart();
                    digitalWrite(DOSING_PUMP_ENABLE_PIN[dosingPumpIdx], HIGH);
                    Dosing_Extr_Cmd[dosingPumpIdx] = 1;
                    // drawWait(dosingPumpIdx);
                }
            }
            else if(cmdStr.equals("CLEAN"))
            {
                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;

                if(dosingPumpIdx < SENSOR_NUM)
                {
                    dosingPump[dosingPumpIdx].setDoseAmount(100);
                    dosingPump[dosingPumpIdx].setDoseWeight(1.0);
                    clearDisplay(dosingPumpIdx);
                    drawDisplay(dosingPumpIdx, "CLEAN", "CLEAN", 100);
                    dosingPump[dosingPumpIdx].start();
                    // drawDone(dosingPumpIdx);
                    char sendStr[40] = {'\0',};
                    sprintf(sendStr, "CLEAN,%d,OK\n", dosingPumpIdx+1);
                    Serial.print(sendStr);
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
            if(Dosing_Extr_Cmd[idx] == 1)
            {
                dosingPump[idx].start();
                // drawExtr(idx);
                char sendStr[40] = {'\0',};
                sprintf(sendStr, "EXTR,%d,0,%d\n", idx+1, dosingPump[idx].getDoseAmount());
                Serial.print(sendStr);
                ledButton[idx].blinkStop();
                ledButton[idx].setLedState(HIGH);
            }
        }
    }
}

void checkDosingPump()
{
    if(dosingPump[0].getDoseStat() == Dosing::RUN || dosingPump[0].getDoseStat() == Dosing::MANUAL ||
       dosingPump[1].getDoseStat() == Dosing::RUN || dosingPump[1].getDoseStat() == Dosing::MANUAL ||
       dosingPump[2].getDoseStat() == Dosing::RUN || dosingPump[2].getDoseStat() == Dosing::MANUAL || 
       dosingPump[3].getDoseStat() == Dosing::RUN || dosingPump[3].getDoseStat() == Dosing::MANUAL ||
       dosingPump[4].getDoseStat() == Dosing::RUN || dosingPump[4].getDoseStat() == Dosing::MANUAL)
    {
        dosingPump[0].upPulse();
        dosingPump[1].upPulse();
        dosingPump[2].upPulse();
        dosingPump[3].upPulse();
        dosingPump[4].upPulse();
        delayMicroseconds(500);
        dosingPump[0].downPulse();
        dosingPump[1].downPulse();
        dosingPump[2].downPulse();
        dosingPump[3].downPulse();
        dosingPump[4].downPulse();
        delayMicroseconds(500);
    }


    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(dosingPump[idx].check() == Dosing::END)
        {
            dosingPump[idx].stop();
            ledButton[idx].setLedState(LOW);
            // drawDone(idx);
            char sendStr[40] = {'\0',};
            sprintf(sendStr, "EXTR,%d,%d,%d\n", idx+1, dosingPump[idx].getDoseAmount(), dosingPump[idx].getDoseAmount());
            Serial.print(sendStr);
            digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], LOW);
        }
        else if(dosingPump[idx].check() == Dosing::STOP)
        {
            if( (Dosing_Extr_Cmd[idx] == 1) && (ledButton[idx].isPushed()==0) )
            {
                Dosing_Extr_Cmd[idx] = 0;
            }
        }
    }
}

void checkManualMode()
{
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(Dosing_Extr_Cmd[idx] == 0)
        {
            if(dosingPump[idx].getDoseStat() == Dosing::STOP && digitalRead(BUTTON_MANUAL_PIN[idx]))
            {
                dosingPump[idx].startManual();
                digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], HIGH);
            }
            else if(dosingPump[idx].getDoseStat() == Dosing::MANUAL && !digitalRead(BUTTON_MANUAL_PIN[idx]))
            {
                dosingPump[idx].stop();
                digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], LOW);
           }
        }
    }
}

void checkEmergency()
{
    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(digitalRead(BUTTON_EMERGENCY_PIN[idx]))
        {
            // 대기중(SET)이거나 추출대기중(EXTR)이거나, 추출중(RUN)인 경우에 비상정지 버튼을 누르는 경우
            if( dosingPump[idx].getDoseStat() == Dosing::SET ||
                dosingPump[idx].getDoseStat() == Dosing::WAIT || 
                dosingPump[idx].getDoseStat() == Dosing::RUN)
            {
                char sendStr[40] = {'\0',};

                // 대기중(SET)이거나 추출대기중(EXTR) 상태(버튼을 누르지 않은 상태)이면, 시작한 것으로 인식하도록 명령을 전달함
                if( dosingPump[idx].getDoseStat() == Dosing::SET ||
                    dosingPump[idx].getDoseStat() == Dosing::WAIT)
                {
                    sprintf(sendStr, "EXTR,%d,0,%d\n", idx+1, dosingPump[idx].getDoseAmount());
                    Serial.print(sendStr);
                    // TODO: Fine tuning
                    delay(1000*2);
                }
                // 완료메세지 전송
                sprintf(sendStr, "EXTR,%d,%d,%d\n", idx+1, dosingPump[idx].getDoseAmount(), dosingPump[idx].getDoseAmount());
                Serial.print(sendStr);
                digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], LOW);
            }
            
            dosingPump[idx].stop();
            ledButton[idx].blinkStop();
            ledButton[idx].setLedState(LOW);
            drawDisplay(idx, "", "", 0);
        }
    }
}

void ledTimerISR()
{
    // Timer Flag
    Timer_Flag = 1;
    Timer_Toggle_Flag = !Timer_Toggle_Flag;

    for(int idx=0; idx<SENSOR_NUM; idx++)
    {
        if(ledButton[idx].getBlinkStarted())
        {
            ledButton[idx].setLedState(Timer_Toggle_Flag);
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
        // Use Emergency Button
        pinMode(BUTTON_EMERGENCY_PIN[idx], INPUT_PULLUP);
        // Use Manual Button
        pinMode(BUTTON_MANUAL_PIN[idx], INPUT_PULLUP);
        // TODO: Enable pin
        pinMode(DOSING_PUMP_ENABLE_PIN[idx], OUTPUT);
        Dosing_Extr_Cmd[idx] = 0;
    }

    // Use for Alive_LED
    pinMode(AliveLedPin, OUTPUT);

    MsTimer2::set(500, ledTimerISR);
    MsTimer2::start();

    //Enable Watchdog
    wdt_enable(WDTO_8S);
}

void loop()
{
    // Check Protocol
    checkProtocol();

    // Check Button Status
    checkButton();

    // Check Dosing Pump
    checkDosingPump();

    // Check Manual Mode
    checkManualMode();

    // Check Emergency Condition
    checkEmergency();

    // Timer Flag
    if(Timer_Flag)
    {
        Timer_Flag = 0;
        // Alive LED
        digitalWrite(AliveLedPin, !(digitalRead(AliveLedPin)));

        //clear watchdog timeout
        wdt_reset();
    }
}
