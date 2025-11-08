// TOF Setting
#define SDA_PIN 21      // SDA Port
#define SCL_PIN 22      // SCL Port
#define LOX1_ADDRESS 0x30     // Address for two sensors
#define LOX2_ADDRESS 0x31
#define SHT_LOX1 18     // Shutdown Pin
#define SHT_LOX2 19
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();     // VL53L0X Object
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
int TOF_Front = 0;
int TOF_Side = 0;

// TOF Functions
void setup_tof() {
    // all reset
    digitalWrite(SHT_LOX1, LOW);    
    digitalWrite(SHT_LOX2, LOW);
    delay(10);
    // all unreset
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, HIGH);
    delay(10);
  
    // activating LOX1 and resetting LOX2
    digitalWrite(SHT_LOX1, HIGH);
    digitalWrite(SHT_LOX2, LOW);
  
    // initing LOX1
    if(!lox1.begin(LOX1_ADDRESS)) {
      Serial.println(F("Failed to boot first VL53L0X"));
      while(1);
    }
    delay(10);
  
    // activating LOX2
    digitalWrite(SHT_LOX2, HIGH);
    delay(10);
  
    //initing LOX2
    if(!lox2.begin(LOX2_ADDRESS)) {
      Serial.println(F("Failed to boot second VL53L0X"));
      while(1);
    }
}
  
void read_dual_sensors() {
    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

    // print sensor one reading
    Serial.print(F("Sensor 1: "));
    if(measure1.RangeStatus != 4) {     // if not out of range
        TOF_Front = measure1.RangeMilliMeter;
        Serial.print(measure1.RangeMilliMeter);
        Serial.print(" mm");
    } else {
        Serial.print(F("Out of Range!"));
    }
    Serial.print(F(", "));

    // print sensor two reading
    Serial.print(F("Sensor 2: "));
    if(measure2.RangeStatus != 4) {
        TOF_Side = measure2.RangeMilliMeter;
        Serial.print(measure2.RangeMilliMeter);
        Serial.print(" mm");
    } else {
        Serial.print(F("Out of Range!"));
    }
    Serial.println(".");
}

void wall_following() {
    // Forward Logic
    if (10 < TOF_Front && TOF_Front < 90000 && 10 < TOF_Side && TOF_Side < 90000) {
      // Working Range for TOF sensor
      if (TOF_Side > 200) {
        // Too far from wall
        // Turn slightly right
        Serial.println("Turn Left A Bit");
        mRight(); 
        ledcAnalogWrite(LEDC_CHANNEL1, 1200); // motor1
        ledcAnalogWrite(LEDC_CHANNEL2, 1200); // motor2
        delay(8);
        mForward();
        delay(10);
        mStop();
      }
      else if (TOF_Side < 100) {
        Serial.println("Too Close to wall");
        mLeftR();
        ledcAnalogWrite(LEDC_CHANNEL1, 1200); // motor1
        ledcAnalogWrite(LEDC_CHANNEL2, 1200); // motor2
        delay(10);
        mForward();
        delay(10);
        mStop();
      }
      else if (TOF_Front > 350) {
        // Front is clear
        Serial.println("Keep moving forward");
        mForward();
        ledcAnalogWrite(LEDC_CHANNEL1, 1200); // motor1
        ledcAnalogWrite(LEDC_CHANNEL2, 1200); // motor2
        delay(50);
        mStop();
  
      }
      else if (250 > TOF_Front) {
        Serial.println("Too Close to wall");
        mBackward();
        ledcAnalogWrite(LEDC_CHANNEL1, 1200); // motor1
        ledcAnalogWrite(LEDC_CHANNEL2, 1200); // motor2
        delay(10);
        mStop();
      }
      else {
        // Reach the end of wall
        // Turn left or right for 90 degree
        Serial.println("Turn Left 90");
        mLeft();
        ledcAnalogWrite(LEDC_CHANNEL1, 800); // motor1
        ledcAnalogWrite(LEDC_CHANNEL2, 800); // motor2
        delay(600);
        mStop();
      }
    }
    else {
      Serial.println("Out of Range");
    } 
}