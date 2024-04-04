#include "OLED.h"
#include "Dosing.h"

Dosing dosingPump[5];
String inputString;

char* DOSING_STATUS_STR[3] = {"WAIT", "RUN", "STOP"};

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
                    dosingPump[dosingPumpIdx].setDoseAmount(doseAmount);
                    clearDisplay(dosingPumpIdx);
                    drawDisplay(dosingPumpIdx, nameStr.c_str(), doseAmount);
                    sprintf(sendStr, "SET,%d,OK,0\n", dosingPumpIdx+1);
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

                if(dosingPumpIdx < 5)
                {
                    unsigned int dosingPumpStat = dosingPump[dosingPumpIdx].check();
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

                if(dosingPumpIdx < 5)
                {
                    dosingPump[dosingPumpIdx].setDoseAmount(doseAmount);
                }
            }
            else if(cmdStr.equals("CLEAN"))
            {
                String dosingPumpIdxStr = inputString.substring(splitIdx1+1, inputString.length());

                unsigned int dosingPumpIdx = dosingPumpIdxStr.toInt()-1;

                if(dosingPumpIdx < 5)
                {
                    dosingPump[dosingPumpIdx].setDoseAmount(200);
                    clearDisplay(dosingPumpIdx);
                    drawDisplay(dosingPumpIdx, "CLEAR", 200);
                }
            }
            inputString = ""; // 입력 문자열 초기화
        }
    }
}

void checkButton()
{

}

void checkDosingPump()
{
    for(int i=0; i<5; i++)
    {
        if(dosingPump[i].check() == Dosing::END)
        {
            dosingPump[i].stop();
            char sendStr[40] = {'\0',};
            sprintf(sendStr, "EXTR,%d,%d,%d\n", i+1, dosingPump[i].getDoseAmount(), dosingPump[i].getDoseAmount());
            Serial.print(sendStr);
        }
    }
}

void setup()
{
    Serial.begin(9600);
    initDisplays();
    dosingPump[0].initPin(2); // TODO: Fix pin
    dosingPump[1].initPin(3); // TODO: Fix pin
    dosingPump[2].initPin(4); // TODO: Fix pin
    dosingPump[3].initPin(5); // TODO: Fix pin
    dosingPump[4].initPin(6); // TODO: Fix pin
}

void loop()
{
    // Check Protocol
    checkProtocol();

    // Check Button Status
    checkButton();

    // Check Dosing Pump
    checkDosingPump();

    // Check Flow Sensor Status

}
