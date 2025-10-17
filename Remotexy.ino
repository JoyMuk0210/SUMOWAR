
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>

#define REMOTEXY_BLUETOOTH_NAME "SUMOXY"

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 86 bytes
  { 255,4,0,0,0,79,0,19,0,0,0,0,165,3,106,200,200,82,200,84,
  1,1,3,0,1,65,154,24,24,140,63,10,10,140,65,10,10,0,2,31,
  72,73,84,0,10,22,153,24,24,59,63,10,10,59,65,10,10,48,4,26,
  31,79,78,0,31,79,70,70,0,5,26,83,60,60,93,34,25,25,93,35,
  26,26,0,2,26,31 };
// uint8_t RemoteXY_CONF[] = {
//   255,3,0,0,0,39,0,19,0,0,0,0,31,1,106,200,1,1,2,0,
//   10,40,115,24,24,48,4,26,31,79,78,0,31,79,70,70,0,5,24,23,
//   60,60,0,2,26,31
// };
struct {

    // input variables
  uint8_t HIT; // =1 if button pressed, else =0, from 0 to 1
  uint8_t pushSwitch_01; // =1 if state is ON, else =0, from 0 to 1
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
// struct {
//   uint8_t pushSwitch_01;
//   int8_t joystick_01_x;
//   int8_t joystick_01_y;
//   uint8_t connect_flag;
// } RemoteXY;
#pragma pack(pop)
#define LEFT_DIR_PIN 14
#define RIGHT_DIR_PIN 26
#define LEFT_PWM_PIN  27
#define RIGHT_PWM_PIN 25

#define LEFT_CH  0
#define RIGHT_CH 1


void setup() {
  Serial.begin(115200);
  RemoteXY_Init();
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  ledcSetup(LEFT_CH, 3000, 8);  // channel 0, 3kHz, 8-bit
  ledcAttachPin(LEFT_PWM_PIN, LEFT_CH);
  ledcSetup(RIGHT_CH, 3000, 8); // channel 1, 3kHz, 8-bit
  ledcAttachPin(RIGHT_PWM_PIN, RIGHT_CH);
  Serial.println("✅ ESP32 BLE RemoteXY Test Ready");
}

void loop() {
  RemoteXY_Handler();
  //put your main code here, to run repeatedly:
  int8_t x = RemoteXY.joystick_01_x;
  int8_t y = RemoteXY.joystick_01_y;
  int deadzone = 15;
  if (abs(x) < deadzone) x = 0;
  if (abs(y) < deadzone) y = 0;
  int16_t leftSpeed  = y - x;
  int16_t rightSpeed = y + x;

  leftSpeed  = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);

  int leftPWM  = map(abs(leftSpeed), 0, 100, 0, 255);
  int rightPWM = map(abs(rightSpeed), 0, 100, 0, 255);
  int minPWM = 60;
  if (leftPWM < minPWM && leftPWM != 0) leftPWM = minPWM;
  if (rightPWM < minPWM && rightPWM != 0) rightPWM = minPWM;

  if (leftSpeed >= 0) {
     digitalWrite(LEFT_DIR_PIN, HIGH);       // Forward
     ledcWrite(LEFT_CH, leftPWM);            // Speed
    } 
    else {
     digitalWrite(LEFT_DIR_PIN, LOW);        // Reverse
     ledcWrite(LEFT_CH, leftPWM);           // Speed (absolute)
    }
 // Right motor
  if (rightSpeed >= 0) {
     digitalWrite(RIGHT_DIR_PIN, HIGH);      // Forward
     ledcWrite(RIGHT_CH, rightPWM);          // Speed
    } 
    else {
     digitalWrite(RIGHT_DIR_PIN, LOW);       // Reverse
     ledcWrite(RIGHT_CH, rightPWM);         // Speed (absolute)
    }
    if(RemoteXY.HIT==1)



  // Print joystick and button values
  Serial.print(" | ON/OFF: ");
  Serial.print(RemoteXY.pushSwitch_01);
  Serial.print(" |Joystick X: ");
  Serial.print(RemoteXY.joystick_01_x);
  Serial.print(" | Y: ");
  Serial.print(RemoteXY.joystick_01_y);
  Serial.print(" | HIT: ");
  Serial.print(RemoteXY.HIT);
  Serial.print(" | Connected: ");
  Serial.println(RemoteXY.connect_flag);

  delay(200); // Slow down serial output for readability
}


// ---
// #define REMOTEXY_MODE__ESP32_BLE
// #define REMOTEXY_BLUETOOTH_NAME "SUMOCAR"
// #include <BLEDevice.h>
// #include <RemoteXY.h>

// // RemoteXY GUI configuration
// #pragma pack(push, 1)
// uint8_t RemoteXY_CONF[] = {
//   255, 4, 0, 0, 0, 51, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 3, 0,
//   1, 64, 146, 24, 24, 0, 2, 31, 72, 73, 84, 0, 10, 27, 148, 24, 24, 48, 4, 26,
//   31, 79, 78, 0, 31, 79, 70, 70, 0, 5, 25, 60, 60, 60, 0, 2, 26, 31
// };


// // RemoteXY variables
// struct {
//   uint8_t HIT;
//   uint8_t pushSwitch_01;
//   int8_t joystick_01_x;
//   int8_t joystick_01_y;
//   uint8_t connect_flag;
// } RemoteXY;
// #pragma pack(pop)

// void setup() {
//   RemoteXY_Init();  // ✅ This works correctly with BLE mode
// }

// void loop() {
//   RemoteXY_Handler();   // ✅ Handles BLE communication
//   RemoteXY_delay(100);  // Use this instead of delay()
// }
