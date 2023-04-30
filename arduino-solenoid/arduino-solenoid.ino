const int SOLENOID_1_PIN = 3;
const int SOLENOID_2_PIN = 4;
const int SOLENOID_3_PIN = 5;
const int SOLENOID_4_PIN = 6;
const int SOLENOID_5_PIN = 7;

const int PHOTO_SENSOR_1_PIN = 8;
const int PHOTO_SENSOR_2_PIN = 9;
const int PHOTO_SENSOR_3_PIN = 10;
const int PHOTO_SENSOR_4_PIN = 11;
const int PHOTO_SENSOR_5_PIN = 12;

const int FLOW_SENSOR_1_PIN = 2; // Interrupt 0
const int FLOW_SENSOR_2_PIN = 18; // Interrupt 5
const int FLOW_SENSOR_3_PIN = 19; // Interrupt 4
const int FLOW_SENSOR_4_PIN = 20; // Interrupt 3
const int FLOW_SENSOR_5_PIN = 21; // Interrupt 5

bool flowSensor1Enabled = false;
bool flowSensor2Enabled = false;
bool flowSensor3Enabled = false;
bool flowSensor4Enabled = false;
bool flowSensor5Enabled = false;

unsigned long flowSensor1Amount = 0;
unsigned long flowSensor2Amount = 0;
unsigned long flowSensor3Amount = 0;
unsigned long flowSensor4Amount = 0;
unsigned long flowSensor5Amount = 0;

// YF-S201
// const float PULSE_PER_MILLILITER = 0.450;
// YF-S401
const float PULSE_PER_MILLILITER = 5.880; // 5.880

// Variable for user input string
String inputString = ""; 

int getSensorCountFromMilliliter(int milliliter)
{
  return round(milliliter * PULSE_PER_MILLILITER);
}

void setup() 
{
  // Set Pin Mode
  pinMode(SOLENOID_1_PIN, OUTPUT);
  pinMode(SOLENOID_2_PIN, OUTPUT);
  pinMode(SOLENOID_3_PIN, OUTPUT);
  pinMode(SOLENOID_4_PIN, OUTPUT);
  pinMode(SOLENOID_5_PIN, OUTPUT);
  pinMode(PHOTO_SENSOR_1_PIN, INPUT);
  pinMode(PHOTO_SENSOR_2_PIN, INPUT);
  pinMode(PHOTO_SENSOR_3_PIN, INPUT);
  pinMode(PHOTO_SENSOR_4_PIN, INPUT);
  pinMode(PHOTO_SENSOR_5_PIN, INPUT);
  pinMode(FLOW_SENSOR_1_PIN, INPUT);
  pinMode(FLOW_SENSOR_2_PIN, INPUT);
  pinMode(FLOW_SENSOR_3_PIN, INPUT);
  pinMode(FLOW_SENSOR_4_PIN, INPUT);
  pinMode(FLOW_SENSOR_5_PIN, INPUT);

  // Set Initial status
  digitalWrite(SOLENOID_1_PIN, HIGH);
  digitalWrite(SOLENOID_2_PIN, HIGH);
  digitalWrite(SOLENOID_3_PIN, HIGH);
  digitalWrite(SOLENOID_4_PIN, HIGH);
  digitalWrite(SOLENOID_5_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_1_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_2_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_3_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_4_PIN, HIGH);
  digitalWrite(FLOW_SENSOR_5_PIN, HIGH);

  // Attach interrupt for Flow Measure
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_1_PIN), FLOW_SENSORE_1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_2_PIN), FLOW_SENSORE_2_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_3_PIN), FLOW_SENSORE_3_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_4_PIN), FLOW_SENSORE_4_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_5_PIN), FLOW_SENSORE_5_ISR, RISING);

  // Enable Interrupt
  sei();

  // Start Serial Communication
  Serial.begin(9600); 
}

void FLOW_SENSORE_1_ISR()
{
  if(flowSensor1Amount <= 0 && flowSensor1Enabled)
  {
    digitalWrite(SOLENOID_1_PIN, HIGH); // Valve close
    flowSensor1Enabled = false;
  }
  flowSensor1Amount--;
}

void FLOW_SENSORE_2_ISR()
{
  if(flowSensor2Amount <= 0 && flowSensor2Enabled)
  {
    digitalWrite(SOLENOID_2_PIN, HIGH); // Valve close
    flowSensor2Enabled = false;
  }
  flowSensor2Amount--;
}

void FLOW_SENSORE_3_ISR()
{
  if(flowSensor3Amount <= 0 && flowSensor3Enabled)
  {
    digitalWrite(SOLENOID_3_PIN, HIGH); // Valve close
    flowSensor3Enabled = false;
  }
  flowSensor3Amount--;
}

void FLOW_SENSORE_4_ISR()
{
  if(flowSensor4Amount <= 0 && flowSensor4Enabled)
  {
    digitalWrite(SOLENOID_4_PIN, HIGH); // Valve close
    flowSensor4Enabled = false;
  }
  flowSensor4Amount--;
}

void FLOW_SENSORE_5_ISR()
{
  if(flowSensor5Amount <= 0 && flowSensor5Enabled)
  {
    digitalWrite(SOLENOID_5_PIN, HIGH); // Valve close
    flowSensor5Enabled = false;
  }
  flowSensor5Amount--;
}

void loop() {
  while (Serial.available()) 
  {
    char inChar = (char)Serial.read(); // 입력된 문자 읽기
    inputString += inChar; // 문자열에 추가
    if (inChar == '\n') // 사용자가 Enter를 입력했을 경우
    {
      int splitterIdx = inputString.indexOf(",");
      String solenoidNumStr = inputString.substring(0, splitterIdx);
      String ammountStr = inputString.substring(splitterIdx+1, inputString.length());
      int solenoidNum = solenoidNumStr.toInt();
      int ammount = ammountStr.toInt();
      inputString = ""; // 입력 문자열 초기화
      digitalWrite(solenoidNum, ammount);
      if(solenoidNum == 1)
      {
        while(digitalRead(PHOTO_SENSOR_1_PIN) == 1) // Cup on Sensor
        {
          delay(1000); // 1초마다 컵이 있는지 검사
        }
        flowSensor1Amount = getSensorCountFromMilliliter(ammount);
        flowSensor1Enabled = true;
        digitalWrite(SOLENOID_1_PIN, LOW);
      }
      else if(solenoidNum == 2)
      {
        while(digitalRead(PHOTO_SENSOR_2_PIN) == 1) // Cup on Sensor
        {
          delay(1000); // 1초마다 컵이 있는지 검사
        }
        flowSensor2Amount = getSensorCountFromMilliliter(ammount);
        flowSensor2Enabled = true;
        digitalWrite(SOLENOID_2_PIN, LOW);

      }
      else if(solenoidNum == 3)
      {
        while(digitalRead(PHOTO_SENSOR_3_PIN) == 1) // Cup on Sensor
        {
          delay(1000); // 1초마다 컵이 있는지 검사
        }
        flowSensor3Amount = getSensorCountFromMilliliter(ammount);
        flowSensor3Enabled = true;
        digitalWrite(SOLENOID_3_PIN, LOW);
      }
      else if(solenoidNum == 4)
      {
        while(digitalRead(PHOTO_SENSOR_4_PIN) == 1) // Cup on Sensor
        {
          delay(1000); // 1초마다 컵이 있는지 검사
        }
        flowSensor4Amount = getSensorCountFromMilliliter(ammount);
        flowSensor4Enabled = true;
        digitalWrite(SOLENOID_4_PIN, LOW);
      }
      else if(solenoidNum == 5)
      {
        while(digitalRead(PHOTO_SENSOR_5_PIN) == 1) // Cup on Sensor
        {
          delay(1000); // 1초마다 컵이 있는지 검사          
        }
        flowSensor5Amount = getSensorCountFromMilliliter(ammount);
        flowSensor5Enabled = true;
        digitalWrite(SOLENOID_5_PIN, LOW);
      }
      else
      {
        // wrong solenoid number
      }      
    }
  }

  delay(100); // 짧은 딜레이
}

// #define FLOWSENSORPIN 9                  // 핀 번호 설정
// volatile uint16_t pulses = 0;               // 데이터 유형 설정
// volatile uint8_t lastflowpinstate;
// volatile uint32_t lastflowratetimer = 0;
// volatile float flowrate;
// SIGNAL(TIMER0_COMPA_vect) {
//   uint8_t x = digitalRead(FLOWSENSORPIN);   // 유량측정센서 값을 디지털로 읽음
//   if (x == lastflowpinstate) {
//     lastflowratetimer++;
//     return;
//   }
//   if (x == HIGH) {                          // x값이 들어오면 pulse값을 1 증가시킴
//     pulses++;
//   }
//   lastflowpinstate = x;
//   flowrate = 1000.0;
//   flowrate /= lastflowratetimer;
//   lastflowratetimer = 0;
// }
// void useInterrupt(boolean v) {             // bool값에 따른 출력 설정
//   if (v) {
//     OCR0A = 0xAF;
//     TIMSK0 |= _BV(OCIE0A);
//   } else {
//     TIMSK0 &= ~_BV(OCIE0A);
//   }
// }
// void setup() {                              // 센서 통신속도 설정 및 입출력 설정
//   Serial.begin(9600);
//   pinMode(FLOWSENSORPIN, INPUT);
//   digitalWrite(FLOWSENSORPIN, HIGH);
//   lastflowpinstate = digitalRead(FLOWSENSORPIN);
//   useInterrupt(true);
// }
// void loop()                                 // 시리얼 모니터에 출력
// {
//   Serial.print("Freq: "); Serial.print(flowrate); Serial.print("  ");
//   Serial.print("Pulses: "); Serial.print(pulses, DEC); Serial.print("  ");
//   float liters = pulses;
//   liters /= 7.5;
//   liters /= 60.0;
//   Serial.print(liters); Serial.println(" Liters");
//   delay(1000);
// }

