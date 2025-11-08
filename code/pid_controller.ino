// MOTOR Setting
#define MOTOR1_PIN1           25
#define MOTOR1_PIN2           26
#define MOTOR2_PIN1           27
#define MOTOR2_PIN2           14
#define ENCDPinMot1           16   // encoder pin of motor1
#define ENCDPinMot2           17  // encoder pin of motor2
#define PWMMoter1             32     // PWM pin of motor1
#define PWMMoter2             33     // PWM pin of motor2

// PID parameters
const int KP = 1.8;
const int KI = 0;
const int KD = 0.1;
int summederror[] = {0,0};
int oldSpeed[] = {0,0};
int desired = 0;  // PWM Value
int desired2 = 0;
int u[] = {0,0};
int u1 = 0;
int u2 = 0;
// Define variable for encoder
const int sensorPins[] = {9,10};  // Sensor pins
unsigned long preTime = 0;
unsigned long curTime = 0;
int numMotors = 2;
int count[] = {0,0};
int speed[] = {0,0};
int currentSensorState[] = {0,0};
int lastSensorState[] = {0,0};

// Motor Functions
void encoder(){
    // Read the current state of the sensor
    curTime = millis();
  
    for (int i=0; i<numMotors; i++){
      currentSensorState[i] = digitalRead(sensorPins[i]);
      if (currentSensorState[i] != lastSensorState[i]) {
        if (currentSensorState[i] == HIGH) {
          count[i]++;
        }
      lastSensorState[i] = currentSensorState[i];
      }
    }
  
    if (curTime - preTime >= 100) {
      for (int i=0; i<numMotors; i++){
        // Calculate speed as interrupts per second
        speed[i] = 60.0/11.0*count[i];
        // Reset the interrupt count and the preTime to the current time
        count[i] = 0;
        preTime = curTime;
      }
    } else if (curTime < preTime) {
      // Handle the case of millis() overflow
      preTime = curTime;
    }
  }

// Similar logic for the rest of the movements
void mStop(){
    digitalWrite(MOTOR1_PIN1, HIGH);
    digitalWrite(MOTOR1_PIN2, HIGH);
    digitalWrite(MOTOR2_PIN1, HIGH);
    digitalWrite(MOTOR2_PIN2, HIGH);
    Serial.println("Stop R");  
}

void setup_motor(){
    // Setup for Motor speed
    ledcSetup(LEDC_CHANNEL1, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
    ledcSetup(LEDC_CHANNEL2, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
    ledcAttachPin(PWMMoter1, LEDC_CHANNEL1);
    ledcAttachPin(PWMMoter2, LEDC_CHANNEL2);
  
    // set motor control pins as output
    pinMode(MOTOR1_PIN1, OUTPUT); 
    pinMode(MOTOR1_PIN2, OUTPUT); 
    pinMode(MOTOR2_PIN1, OUTPUT);
    pinMode(MOTOR2_PIN2, OUTPUT); 
  }
  
  void PIDControl() {
    // Iterate over each motor in the system
    for (int i = 0; i < numMotors; i++) {
      // Calculate the velocity, which is the change in speed
      int velocity = speed[i] - oldSpeed[i];
      // Update the cumulative error by adding the current error (difference between desired speed and current speed)
      summederror[i] += (desired - speed[i]);
      // PID Control Calculation:
      u[i] = KP * (desired - speed[i]) + KD * velocity + KI * summederror[i];
      // Adjust summed error if the motor speed is more than desired speed
      if (desired - speed[i] < 0) summederror[i] *= 0.5;
      // Limit the maximum value of summed error to prevent integral wind-up
      if (summederror[i] > 3000) summederror[i] = 3000;
      // Limit the control signal to a maximum value
      if (u[i] > 1500) u[i] = 1500;
      // Ensure the control signal is not negative
      if (u[i] < 0) u[i] = 0;
      // Update oldSpeed for the next iteration
      oldSpeed[i] = speed[i];
    }
  }