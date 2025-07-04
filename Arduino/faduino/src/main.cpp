#include <MsTimer2.h>
#include <avr/wdt.h>
#include <SimpleModbusSlave.h>

#include <OLED.h>
#include <Dosing.h>
#include <Button.h>

// --- 시스템 설정 ---
#define NUM_OUTLETS 5
#define REGS_PER_OUTLET 28
#define TOTAL_REGS (NUM_OUTLETS * REGS_PER_OUTLET)

// --- Modbus 레지스터 오프셋 정의 (블록 내 상대 주소) ---
#define OFFSET_CMD      0
#define OFFSET_STATUS   1
#define OFFSET_DOSE     2
#define OFFSET_WEIGHT   3
#define OFFSET_USER_NAME 4
#define OFFSET_DRUG_NAME 16

// --- 상태(status) 및 명령(cmd) 정의 ---
#define STATUS_STOPPED 0
#define STATUS_WAITING 1
#define STATUS_EXTRACTING 2
#define STATUS_COMPLETE 3
#define STATUS_MANUAL 4

#define CMD_DRUG_SETTING 1
#define CMD_CLEANING 2

// --- 핀 정의 ---
// Define Dosing Pump Pulse PINs: OUTPUT
const int DOSING_PUMP_PIN[NUM_OUTLETS]          = {6, 7, 8, 9, 10}; // {2, 3, 4, 5, 6}; for Mega
// Define Dosing Pump Enable PINs: OUTPUT
const int DOSING_PUMP_ENABLE_PIN[NUM_OUTLETS]   = {43, 44, 45, 46, 47};
// Define Button LED PINs: OUTPUT
const int BUTTON_LED_PIN[NUM_OUTLETS]           = {38, 39, 40, 41, 42}; // {31, 33, 35, 37, 39}; for Mega
// Define Alive LED PINs: OUTPUT
const int ALIVE_LED_PIN                         = 51;
// Define Button Input PINs: INPUT
const int BUTTON_PUSH_PIN[NUM_OUTLETS]          = {37, 36, 35, 34, 33}; // {30, 32, 34, 36, 38}; for Mega
// Define Emergency Input PINs: INPUT
const int BUTTON_EMERGENCY_PIN[NUM_OUTLETS]     = {26, 25, 24, 23, 22};
// Define Manual Input PINs: INPUT
const int BUTTON_MANUAL_PIN[NUM_OUTLETS]        = {32, 31, 30, 29, 28}; // {7, 8, 9, 10, 11}; for Mega


// --- 전역 변수 ---
Dosing dosingPump[5];
Button ledButton[5];
unsigned int holdingRegs[TOTAL_REGS];

// --- 함수 프로토타입 ---
void handle_outlet_logic(int outlet_index);
void ledTimerISR();
void runDosingPump();
void checkManualMode();
void checkEmergency();

void setup()
{
    // 디스플레이 초기화
    initDisplays();

    // 입/출력 핀 설정
    for (int idx = 0; idx < NUM_OUTLETS; idx++)
    {
        digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], HIGH);
        dosingPump[idx].initPin(DOSING_PUMP_PIN[idx]);
        ledButton[idx].initPin(BUTTON_PUSH_PIN[idx], BUTTON_LED_PIN[idx]);
        // Use Emergency Button
        pinMode(BUTTON_EMERGENCY_PIN[idx], INPUT_PULLUP);
        // Use Manual Button
        pinMode(BUTTON_MANUAL_PIN[idx], INPUT_PULLUP);
        // TODO: Enable pin
        pinMode(DOSING_PUMP_ENABLE_PIN[idx], OUTPUT);
    }

    // Use for Alive_LED
    pinMode(ALIVE_LED_PIN, OUTPUT);

    MsTimer2::set(500, ledTimerISR);
    MsTimer2::start();

    // Modbus 설정 (총 140개 레지스터)
    modbus_configure(9600, 1, 0, TOTAL_REGS, 0);

    // 모든 레지스터 블록 초기화
    for (int i = 0; i < NUM_OUTLETS; i++)
    {
        int base_addr = i * REGS_PER_OUTLET;
        holdingRegs[base_addr + OFFSET_CMD] = 0;
        holdingRegs[base_addr + OFFSET_STATUS] = STATUS_STOPPED;
    }

    // Enable Watchdog
    wdt_enable(WDTO_8S);
}

void loop()
{
    // 1. Modbus 요청 처리 (전체 레지스터 대상)
    modbus_update(holdingRegs);

    // 2. 각 토출구의 로직을 순차적으로 처리
    for (int i = 0; i < NUM_OUTLETS; i++)
    {
        handle_outlet_logic(i);
    }

    // Check Dosing Pump
    runDosingPump();

    // Check Manual Mode
    checkManualMode();

    // Check Emergency Condition
    checkEmergency();

    wdt_reset();
}

// 단일 토출구의 전체 로직을 처리하는 함수
void handle_outlet_logic(int outlet_index)
{
    int base_addr = outlet_index * REGS_PER_OUTLET;

    // --- 1. Modbus 명령 처리 ---
    unsigned int cmd = holdingRegs[base_addr + OFFSET_CMD];
    if (cmd != 0)
    {
        if (cmd == CMD_DRUG_SETTING && (holdingRegs[base_addr + OFFSET_STATUS] != STATUS_EXTRACTING && holdingRegs[base_addr + OFFSET_STATUS] != STATUS_MANUAL))
        {
            // Modbus 레지스터에서 값 읽기
            unsigned int doseAmount = holdingRegs[base_addr + OFFSET_DOSE];
            float doseWeight = (float)holdingRegs[base_addr + OFFSET_WEIGHT] / 100.0; // weight는 100을 곱해서 저장했다고 가정
            
            // user_name (12개 레지스터)
            char userNameBuffer[25]; // 24바이트 + 널 종료 문자
            for (int i = 0; i < 12; ++i) {
                userNameBuffer[i*2] = (holdingRegs[base_addr + OFFSET_USER_NAME + i] >> 8) & 0xFF;
                userNameBuffer[i*2 + 1] = holdingRegs[base_addr + OFFSET_USER_NAME + i] & 0xFF;
            }
            userNameBuffer[24] = '\0'; // 널 종료
            String userNameStr = String(userNameBuffer);

            // drug_name (12개 레지스터)
            char drugNameBuffer[25]; // 24바이트 + 널 종료 문자
            for (int i = 0; i < 12; ++i) {
                drugNameBuffer[i*2] = (holdingRegs[base_addr + OFFSET_DRUG_NAME + i] >> 8) & 0xFF;
                drugNameBuffer[i*2 + 1] = holdingRegs[base_addr + OFFSET_DRUG_NAME + i] & 0xFF;
            }
            drugNameBuffer[24] = '\0'; // 널 종료
            String drugNameStr = String(drugNameBuffer);

            dosingPump[outlet_index].setDoseAmount(doseAmount);
            dosingPump[outlet_index].setDoseWeight(doseWeight);
            ledButton[outlet_index].blinkStart();
            digitalWrite(DOSING_PUMP_ENABLE_PIN[outlet_index], LOW);
            clearDisplay(outlet_index);
            drawDisplay(outlet_index, userNameStr.c_str(), drugNameStr.c_str(), doseAmount);

            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_WAITING;
        }
        else if (cmd == CMD_CLEANING && (holdingRegs[base_addr + OFFSET_STATUS] == STATUS_STOPPED || holdingRegs[base_addr + OFFSET_STATUS] == STATUS_COMPLETE))
        {
            dosingPump[outlet_index].setDoseAmount(500);
            dosingPump[outlet_index].setDoseWeight(1.0);
            clearDisplay(outlet_index);
            drawDisplay(outlet_index, "CLEAN", "CLEAN", 500);
            dosingPump[outlet_index].start();
        }
        // 명령 처리 후 초기화
        holdingRegs[base_addr + OFFSET_CMD] = 0;
    }

    // --- 2. 상태 천이 로직 처리 ---
    unsigned int current_status = holdingRegs[base_addr + OFFSET_STATUS];
    bool button_pressed = (ledButton[outlet_index].isPushed());

    switch (current_status)
    {
    case STATUS_STOPPED:
        // IDLE....
        break;

    case STATUS_WAITING:
        if (button_pressed)
        {
            dosingPump[outlet_index].start();
            ledButton[outlet_index].blinkStop();
            ledButton[outlet_index].setLedState(HIGH);
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_EXTRACTING;
        }
        break;

    case STATUS_EXTRACTING:
        if (dosingPump[outlet_index].check() == Dosing::END)
        {
            dosingPump[outlet_index].stop();
            ledButton[outlet_index].setLedState(LOW);
            digitalWrite(DOSING_PUMP_ENABLE_PIN[outlet_index], HIGH);
            delay(100);
            drawDisplay(outlet_index, "", "", 0);
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_COMPLETE;
        }
        break;

    case STATUS_COMPLETE:
        if (button_pressed)
        {
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_STOPPED;
        }
        break;

    case STATUS_MANUAL:
        break;
    }
}

void runDosingPump()
{
    if (dosingPump[0].getDoseStat() == Dosing::RUN || dosingPump[0].getDoseStat() == Dosing::MANUAL ||
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
}

void checkManualMode()
{
    for (int idx = 0; idx < NUM_OUTLETS; idx++)
    {
        int base_addr = idx * REGS_PER_OUTLET;
        // Condition to START manual mode:
        // Pump is STOP or WAIT state AND manual button is pressed
        if ((dosingPump[idx].getDoseStat() == Dosing::STOP ||
             dosingPump[idx].getDoseStat() == Dosing::WAIT) &&
            digitalRead(BUTTON_MANUAL_PIN[idx]))
        {
            dosingPump[idx].startManual();                  // Set pump state to MANUAL
            digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], LOW); // Enable the pump motor for manual control
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_MANUAL;
        }
        // Condition to STOP manual mode:
        // Pump is in MANUAL state AND manual button is released
        else if (dosingPump[idx].getDoseStat() == Dosing::MANUAL && !digitalRead(BUTTON_MANUAL_PIN[idx]))
        {
            dosingPump[idx].stop();
            digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], HIGH); // Disable the pump motor immediately
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_STOPPED;
        }
    }
}

void checkEmergency()
{
    for (int idx = 0; idx < NUM_OUTLETS; idx++)
    {
        if (digitalRead(BUTTON_EMERGENCY_PIN[idx]))
        {
            // Stop the dosing pump immediately
            dosingPump[idx].stop();
            digitalWrite(DOSING_PUMP_ENABLE_PIN[idx], HIGH); // Disable the pump motor

            // Stop LED blinking and turn off LED
            ledButton[idx].blinkStop();
            ledButton[idx].setLedState(LOW);

            // Clear display for this outlet
            drawDisplay(idx, "", "", 0);

            // Update Modbus status register to STOPPED
            int base_addr = idx * REGS_PER_OUTLET;
            holdingRegs[base_addr + OFFSET_STATUS] = STATUS_STOPPED;
        }
    }
}

void ledTimerISR()
{
    // Button LED
    for (int idx = 0; idx < NUM_OUTLETS; idx++)
    {
        if (ledButton[idx].getBlinkStarted())
        {
            ledButton[idx].setLedState(!ledButton[idx].getLedState());
        }
    }
    // Alive LED
    digitalWrite(ALIVE_LED_PIN, !(digitalRead(ALIVE_LED_PIN)));
}