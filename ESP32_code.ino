
// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// including nessecary libraries
#include <RemoteXY.h>
#define REMOTEXY_MODE__WIFI_POINT
#include <WiFi.h>

//defining WIFi access point
#define REMOTEXY_WIFI_SSID "GIGACHAD"
#define REMOTEXY_WIFI_PASSWORD "password"
#define REMOTEXY_SERVER_PORT 6377

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 52 bytes
  { 255,3,0,4,0,45,0,19,0,0,0,0,31,1,106,200,1,1,3,0,
  5,20,83,69,69,32,2,26,31,67,36,63,40,10,78,2,26,2,1,73,
  12,24,24,0,1,31,82,69,83,69,84,0 };
  
// this structure defines all the variables and events of your control interface 
struct {
    // input variables
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  uint8_t button_02; // =1 if button pressed, else =0, from 0 to 1

    // output variables
  float POINTS;
  float points;

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
//defining motor pwm,direction pins and  channels
#define LEFT_DIR_PIN 14
#define RIGHT_DIR_PIN 26
#define LEFT_PWM_PIN  27
#define RIGHT_PWM_PIN 25


#define LEFT_CH  0
#define RIGHT_CH 1

#define irpin 18

void setup() 
{
  //defining INPUT/OUTPUT pins and setup channels
  RemoteXY_Init (); 
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  ledcSetup(LEFT_CH, 3000, 8);  // channel 0, 3kHz, 8-bit
  ledcAttachPin(LEFT_PWM_PIN, LEFT_CH);
  ledcSetup(RIGHT_CH, 3000, 8); // channel 1, 3kHz, 8-bit
  ledcAttachPin(RIGHT_PWM_PIN, RIGHT_CH);
  
  Serial.begin(115200);
  Serial.println("✅ ESP32 WiFi RemoteXY Test Ready");
  points=0;
  

  Serial.println("SumoBot IR Sensor System Initialized!");
  
}

void loop() 
{ 
  RemoteXY_Handler ();
   //Motor operation in defferential drive
  int8_t y = RemoteXY.joystick_01_x/2;
  int8_t x = RemoteXY.joystick_01_y/2;
  int deadzone = 15;
  if (abs(x) < (deadzone/2)) x = 0;
  if (abs(y) < (deadzone/2)) y = 0;
  int16_t leftSpeed  = y - x;
  int16_t rightSpeed = y + x;

  leftSpeed  = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);

  int leftPWM  = map(abs(leftSpeed), 0, 100, 0, 255);
  int rightPWM = map(abs(rightSpeed), 0, 100, 0, 255);
  int minPWM = 10;
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

  //input from arduino read and points decreased if black danger zone detected by IR sensors
  int ir = digitalRead(irpin);

  if(ir==1)
  {
    points=points-1;
    Serial.println("🚫Danger Zone Detected!Penalty!");
    Serial.print("Points:");
    Serial.println(points);
    
    delay(20);
  }
   
  // Print joystick and button values on GUI
  Serial.print(" |Joystick X: ");
  Serial.print(RemoteXY.joystick_01_x);
  Serial.print(" | Y: ");
  Serial.print(RemoteXY.joystick_01_y);
  Serial.print(" | Connected: ");
  Serial.println(RemoteXY.connect_flag);

  //Reset points after every game
  if(RemoteXY.button_02==1)
  {
    points=0;
  }
  RemoteXY.POINTS = points;

}
