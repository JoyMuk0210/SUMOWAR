// #include<Servo.h>
#define NUM_SENSORS 6
 float count;
 // int servoSigPin = 13;
 // int servoSig;
 int irSig = 10 ;

 // Servo servo1;


 // Assign pins for IR sensors (adjust based on your wiring)
 int irPins[NUM_SENSORS] = {4,5,6,7,8,9};

 // Store sensor states
 int irState[NUM_SENSORS];

void setup() {
   pinMode(irSig, OUTPUT);
  // pinMode(servoSigPin, INPUT);

  // servo1.attach(11);
   Serial.begin(115200);
  count=0;
  // Initialize sensor pins
   for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(irPins[i], INPUT);
  }

  Serial.println("SumoBot IR Sensor System Initialized!");
}

void loop() {

  bool dangerDetected = false;

  for (int i = 0; i < NUM_SENSORS; i++) {
    irState[i] = digitalRead(irPins[i]);

    // IR module: LOW = black detected
    if (irState[i] == HIGH) {
      dangerDetected = true;
      Serial.print("⚠  Black zone detected by sensor ");
      Serial.println(i + 1);
    }
  }

  if (dangerDetected) {
    count=count+1;
    Serial.println("🚫Danger Zone Detected!Penalty!");
    digitalWrite(irSig,HIGH);

    Serial.print("Points:");
    Serial.println(count);
    // You can add motor stop or scoring logic here
  } else {
    Serial.println("✅ All Clear (Safe Zone)");
    digitalWrite(irSig,LOW);
  }

  delay(500);

  // servoSig = digitalRead(servoSigPin);

  // if(servoSig == HIGH){
  //   servo1.write(90);
  // }
  // else{
  //   servo1.write(0);
  // }
}
