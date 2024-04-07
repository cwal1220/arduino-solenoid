// Define Dosing Pump PINs: OUTPUT
const int DOSING_PUMP_1_PIN = 3;
const int DOSING_PUMP_2_PIN = 4;
const int DOSING_PUMP_3_PIN = 5;
const int DOSING_PUMP_4_PIN = 6;
const int DOSING_PUMP_5_PIN = 7;

// Define Button LED PINs: OUTPUT
const int BUTTON_LED_1_PIN = 1;
const int BUTTON_LED_2_PIN = 1;
const int BUTTON_LED_3_PIN = 1;
const int BUTTON_LED_4_PIN = 1;
const int BUTTON_LED_5_PIN = 1;

// Define Button Input PINs: INPUT
const int BUTTON_PUSH_1_PIN = 8;
const int BUTTON_PUSH_2_PIN = 9;
const int BUTTON_PUSH_3_PIN = 10;
const int BUTTON_PUSH_4_PIN = 11;
const int BUTTON_PUSH_5_PIN = 12;

// Define Flower sensor PINs: INPUT
const int FLOW_SENSOR_1_PIN = 2;  // Interrupt 0
const int FLOW_SENSOR_2_PIN = 18; // Interrupt 5
const int FLOW_SENSOR_3_PIN = 19; // Interrupt 4
const int FLOW_SENSOR_4_PIN = 20; // Interrupt 3
const int FLOW_SENSOR_5_PIN = 21; // Interrupt 5

// Variable for user input string
String inputString = "";

void setup()
{
    // Set Pin Mode
    pinMode(DOSING_PUMP_1_PIN, OUTPUT);
    pinMode(DOSING_PUMP_2_PIN, OUTPUT);
    pinMode(DOSING_PUMP_3_PIN, OUTPUT);
    pinMode(DOSING_PUMP_4_PIN, OUTPUT);
    pinMode(DOSING_PUMP_5_PIN, OUTPUT);
    pinMode(BUTTON_LED_1_PIN, OUTPUT);
    pinMode(BUTTON_LED_2_PIN, OUTPUT);
    pinMode(BUTTON_LED_3_PIN, OUTPUT);
    pinMode(BUTTON_LED_4_PIN, OUTPUT);
    pinMode(BUTTON_LED_5_PIN, OUTPUT);
    pinMode(BUTTON_PUSH_1_PIN, INPUT);
    pinMode(BUTTON_PUSH_2_PIN, INPUT);
    pinMode(BUTTON_PUSH_3_PIN, INPUT);
    pinMode(BUTTON_PUSH_4_PIN, INPUT);
    pinMode(BUTTON_PUSH_5_PIN, INPUT);
    pinMode(FLOW_SENSOR_1_PIN, INPUT);
    pinMode(FLOW_SENSOR_2_PIN, INPUT);
    pinMode(FLOW_SENSOR_3_PIN, INPUT);
    pinMode(FLOW_SENSOR_4_PIN, INPUT);
    pinMode(FLOW_SENSOR_5_PIN, INPUT);

    // Set Initial status
    digitalWrite(DOSING_PUMP_1_PIN, LOW);
    digitalWrite(DOSING_PUMP_2_PIN, LOW);
    digitalWrite(DOSING_PUMP_3_PIN, LOW);
    digitalWrite(DOSING_PUMP_4_PIN, LOW);
    digitalWrite(DOSING_PUMP_5_PIN, LOW);
    digitalWrite(BUTTON_LED_1_PIN, LOW);
    digitalWrite(BUTTON_LED_2_PIN, LOW);
    digitalWrite(BUTTON_LED_3_PIN, LOW);
    digitalWrite(BUTTON_LED_4_PIN, LOW);
    digitalWrite(BUTTON_LED_5_PIN, LOW);

    // Start Serial Communication
    Serial.begin(9600);
}
