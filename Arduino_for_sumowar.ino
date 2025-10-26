//Arduino UNO code to send signal to esp32 if any IR senses danger  zone

//defining pins and variables
#define NUM_SENSORS 6
 float count;
 int irSig = 10 ;
 int irPins[NUM_SENSORS] = {4,5,6,7,8,9};

 // Store sensor states
 int irState[NUM_SENSORS];

void setup() {
   pinMode(irSig, OUTPUT);
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

 //state of IR recorded
  for (int i = 0; i < NUM_SENSORS; i++) {
    irState[i] = digitalRead(irPins[i]);

    if (irState[i] == HIGH) {
      dangerDetected = true;
      Serial.print("⚠  Black zone detected by sensor ");
      Serial.println(i + 1);
    }
  }
//sends signal to esp32 if any IR detects danger zone
  if (dangerDetected) {
    Serial.println("🚫Danger Zone Detected!Penalty!");
    digitalWrite(irSig,HIGH);
  } 
  else {
    Serial.println("✅ All Clear (Safe Zone)");
    digitalWrite(irSig,LOW);
  }

  delay(500);
}
