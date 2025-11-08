/*
Version 12.12.18 (Final)
*/

// General Library
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <esp_now.h>
// Class and Installed Library
#include "html510.h"
#include "Adafruit_VL53L0X.h"
#include "vive510.h"

HTML510Server team24(80);    // Server Object for port 80 initialation the object with my name
// LEDC Setting
#define LEDC_RESOLUTION_BITS  14 // 2^13 = 4096
#define LEDC_RESOLUTION       ((1<<LEDC_RESOLUTION_BITS)-1) //4095
#define LEDC_FREQ_HZ          500
#define LEDC_CHANNEL1         0
#define LEDC_CHANNEL2         1
#define LEDC_VALUE_MAX        1500
// MOTOR Setting
#define MOTOR1_PIN1           25
#define MOTOR1_PIN2           26
#define MOTOR2_PIN1           27
#define MOTOR2_PIN2           14
#define ENCDPinMot1           16   // encoder pin of motor1
#define ENCDPinMot2           17  // encoder pin of motor2
#define PWMMoter1             32     // PWM pin of motor1
#define PWMMoter2             33     // PWM pin of motor2
// IR Setting
const int IRpins[] = {34,35};
int isHighPulse[] = {0,0};
unsigned long pulseDuration[] = {0,0};
float frequency[] = {0,0};
unsigned long lastAnalogValue[] = {0, 0};
int highThreshold = 2500; // Max 4096
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
// WiFi Configuration
const char* ssid     = "Team Pao";
const char* password = "987654321";
WiFiUDP UDPTestServer;
IPAddress ipTarget(192, 168, 1, 255); // 255 => broadcast
IPAddress myIP(192, 168, 1, 217); // change to your IP
// Vive Wifi
#define RGBLED 2 // for ESP32S2 Devkit pin 2, for M5 stamp=2
#define SIGNALPIN1 13 // pin receiving signal from Vive circuit
#define UDPPORT 2510 // For GTA 2022C game 
#define teamNumber 24
#define FREQ 1 // in Hz
Vive510 vive1(SIGNALPIN1);
long int vive_ms = millis();
int police_x = 0;
int police_y = 0;
int vive_x = 0;
int vive_y = 0;

// ESPNow Parameters
#define CHANNEL 0                  // channel can be 1 to 14, channel 0 means current channel.  
// #define MAC_RECV  {0xa0,0x76,0x4e,0x19,0xea,0x2c} // receiver MAC address (last digit should be even for STA)
// Ryan Mac Address
// #define MAC_RECV  {0x68,0x67,0x25,0x82,0x3F,0x8C}
#define MAC_RECV  {0x60,0x55,0xF9,0x57,0x47,0x4C}
esp_now_peer_info_t peer1 = 
{
  .peer_addr = MAC_RECV, 
  .channel = CHANNEL,
  .encrypt = false,
};
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
// Rover Mode
int op_mode = 0; // Default: 0, Wall Following: 1, IR Seeking: 2, Police Car: 3

// Web Design
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Online HTML Editor</title>
    <style>
        /* Button styling */
        .button {
          padding: 15px 25px;
          font-size: 16px;
          text-align: center;
          cursor: pointer;
          outline: none;
          color: #fff;
          background-color: #04AA6D;
          border: none;
          border-radius: 5px;
          width: 150px;
          height: 50px;
          /* You can add more styling if needed, e.g., padding, margin, etc. */
          margin: 5px; /* Add margin for spacing between buttons */
        }
        
        .button1 {
          padding: 15px 25px;
          font-size: 16px;
          text-align: center;
          cursor: pointer;
          outline: none;
          color: #fff;
          background-color: #F70D1A;
          border: none;
          border-radius: 5px;
          width: 315px;
          height: 50px;
          /* You can add more styling if needed, e.g., padding, margin, etc. */
          margin: 5px; /* Add margin for spacing between buttons */
        }
   
        .button:hover {background-color: #3e8e41}
        .button1:hover {background-color: #990000}
   
        .button:active {
          background-color: #3e8e41;
          box-shadow: 0 5px #666;
          transform: translateY(4px);
        }
        .button1:active {
          background-color: #990000;
          box-shadow: 0 5px #666;
          transform: translateY(4px);
        }
        .slidecontainer {
          width: 100%; /* Width of the outside container */
        }
       
        /* The slider itself */
        .slider {
          -webkit-appearance: none;  /* Override default CSS styles */
          appearance: none;
          width: 60%; /* Full-width */
          height: 25px; /* Specified height */
          background: #d3d3d3; /* Grey background */
          outline: none; /* Remove outline */
          opacity: 0.7; /* Set transparency (for mouse-over effects on hover) */
          -webkit-transition: .2s; /* 0.2 seconds transition on hover */
          transition: opacity .2s;
        }
       
        /* Mouse-over effects */
        .slider:hover {
          opacity: 1; /* Fully shown on mouse-over */
        }
       
        /* The slider handle (use -webkit- (Chrome, Opera, Safari, Edge) and -moz- (Firefox) to override default look) */
        .slider::-webkit-slider-thumb {
          -webkit-appearance: none; /* Override default look */
          appearance: none;
          width: 25px; /* Set a specific slider handle width */
          height: 25px; /* Slider handle height */
          background: #04AA6D; /* Green background */
          cursor: pointer; /* Cursor on hover */
        }
       
        .slider::-moz-range-thumb {
          width: 25px; /* Set a specific slider handle width */
          height: 25px; /* Slider handle height */
          background: #04AA6D; /* Green background */
          cursor: pointer; /* Cursor on hover */
        }
    </style>
</head>
<body>
    <h1>Team 24 Final Project</h1>
    <button id="forwardButton" class="button">Forward</button>
    <button id="backwardButton" class="button">Backward</button><br>
    <button id="leftButton" class="button">Left</button>
    <button id="rightButton" class="button">Right</button> <br>
    <button id="leftRButton" class="button">LeftR</button>
    <button id="rightRButton" class="button">RightR</button> 
    
    <div>Duty Cycle 1 Controller</div>
    <div class="slidecontainer">
      <input type="range" min="1" max="100" value="50" class="slider" id="DutyCyc">
      <span id="DutyCycValue">50</span>%
    </div>

    <div>Duty Cycle 2 Controller</div>
    <div class="slidecontainer">
      <input type="range" min="1" max="100" value="50" class="slider" id="DutyCyc2">
      <span id="DutyCycValue2">50</span>%
    </div>
    
    <br>
    <button id="stopButton" class="button1">Stop</button>
    <br>
    <button id="mode_def" class="button">Default</button>
    <button id="mode_wall" class="button">Wall</button> 
    <button id="mode_ir" class="button">IR</button> 
    <button id="mode_pol" class="button">Police</button> 

    <script>
    document.getElementById('forwardButton').addEventListener('click', function() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Forward", true);
    xhr.send();
    });

    document.getElementById('backwardButton').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/Backward", true);
        xhr.send();
    });

    document.getElementById('rightButton').addEventListener('click', function() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Right", true);
    xhr.send();
    });

    document.getElementById('leftButton').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/Left", true);
        xhr.send();
    });
    
    document.getElementById('rightRButton').addEventListener('click', function() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/RightR", true);
    xhr.send();
    });

    document.getElementById('leftRButton').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/LeftR", true);
        xhr.send();
    });
    
    document.getElementById('stopButton').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/Stop", true);
        xhr.send();
    });

    document.getElementById('mode_def').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/M_Def", true);
        xhr.send();
    });

    document.getElementById('mode_wall').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/M_Wall", true);
        xhr.send();
    });

    document.getElementById('mode_ir').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/M_IR", true);
        xhr.send();
    });

    document.getElementById('mode_pol').addEventListener('click', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/M_Pol", true);
        xhr.send();
    });

    document.addEventListener('keyup', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/Stop", true);
        xhr.send();
    });

    document.body.addEventListener('keydown', function(event) {
      var xhr = new XMLHttpRequest();

      switch (event.key.toUpperCase()) {
          case 'W':
              xhr.open("GET", "/Forward", true);
              xhr.send();
              break;
          case 'S':
              xhr.open("GET", "/Backward", true);
              xhr.send();
              break;
          case 'A':
              xhr.open("GET", "/LeftR", true);
              xhr.send();
              break;
          case 'D':
              xhr.open("GET", "/RightR", true);
              xhr.send();
              break;
          case ' ':
              xhr.open("GET", "/Stop", true);
              xhr.send();
              break;
      }
    });

    document.body.addEventListener('keydown', function(event) {
    var xhr = new XMLHttpRequest();

    // Check if the pressed key is one of the arrow keys
    if (event.key === 'o' || event.key === 'k') {
        // Increment or decrement the slider value
        var step = event.key === 'o' ? 1 : -1;
        pwmSlider.value = Math.min(100, Math.max(0, parseInt(pwmSlider.value) + step));

        // Update the displayed value
        pwmOutput.innerHTML = pwmSlider.value;

        // Send the updated value to the server
        xhr.open("GET", "/sliderDutyCyc?val=" + pwmSlider.value, true);
        xhr.send();

        // Prevent the default behavior of arrow keys (e.g., scrolling the page)
        event.preventDefault();
    }
    if (event.key === 'p' || event.key === 'l') {
        // Increment or decrement the slider value
        var step = event.key === 'p' ? 1 : -1;
        pwmSlider2.value = Math.min(100, Math.max(0, parseInt(pwmSlider2.value) + step));

        // Update the displayed value
        pwmOutput2.innerHTML = pwmSlider2.value;

        // Send the updated value to the server
        xhr.open("GET", "/sliderDutyCyc2?val=" + pwmSlider2.value, true);
        xhr.send();

        // Prevent the default behavior of arrow keys (e.g., scrolling the page)
        event.preventDefault();
    }
    });

    // PWM Slider
    var pwmSlider = document.getElementById("DutyCyc");
    var pwmOutput = document.getElementById("DutyCycValue");
    pwmOutput.innerHTML = pwmSlider.value; // Display the default slider value
    pwmSlider.oninput = function() {
        pwmOutput.innerHTML = this.value;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/sliderDutyCyc?val=" + this.value, true);
        xhr.send();
    }; 

    var pwmSlider2 = document.getElementById("DutyCyc2");
    var pwmOutput2 = document.getElementById("DutyCycValue2");
    pwmOutput2.innerHTML = pwmSlider2.value; // Display the default slider value
    pwmSlider2.oninput = function() {
        pwmOutput2.innerHTML = this.value;
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/sliderDutyCyc2?val=" + this.value, true);
        xhr.send();
    }; 
    </script>
</body>
</html>
)===";

// Handler Functions
// HTML content Handle
void handleRoot(){
  team24.sendhtml(body);
}

//Forward Handle
void handleForward(){
  mForward();
  team24.sendhtml(body);
  // espSend();
}

//Backward Handle
void handleBackward(){
  mBackward();
  team24.sendhtml(body);
  // espSend();
}

// One Spin one idle
// Left Handle 
void handleLeft(){
  mLeft();
  team24.sendhtml(body);
  // espSend();
}

// Right Handle
void handleRight(){
  mRight();
  team24.sendhtml(body);
  // espSend();
}

// R for revolute
void handleLeftR(){
  mLeftR();
  team24.sendhtml(body);
  // espSend();
}

// Right Handle
void handleRightR(){
  mRightR();
  team24.sendhtml(body);
  // espSend();
}

void handleStop(){
  mStop();
  team24.sendhtml(body);
  // espSend();
}

//Duty Cycle Handle
void handleSliderDutyCyc(){
  desired = LEDC_VALUE_MAX * team24.getVal()/100; // Desired desiredDutyCyc
  Serial.printf("The desiredDutyCyc is: ");
  Serial.println(desired);
  // espSend();
}

void handleSliderDutyCyc2(){
  desired2 = LEDC_VALUE_MAX * team24.getVal()/100; // Desired desiredDutyCyc
  Serial.printf("The desiredDutyCyc2 is: ");
  Serial.println(desired2);
  // espSend();
}

void handleMode_Def() {
  op_mode = 0;
  team24.sendhtml(body);
  Serial.println("Switch to Default");
  espSend();
}

void handleMode_Wall() {
  op_mode = 1;
  team24.sendhtml(body);
  espSend();
}

void handleMode_IR() {
  op_mode = 2;
  team24.sendhtml(body);
  espSend();
}

void handleMode_Pol() {
  op_mode = 3;
  team24.sendhtml(body);
  Serial.println("Switch to Police Mode");
  espSend();
}

void setup_handler(){
  team24.attachHandler("/", handleRoot);
  team24.attachHandler("/Forward", handleForward);
  team24.attachHandler("/Backward", handleBackward);
  team24.attachHandler("/Right", handleRight);
  team24.attachHandler("/Left", handleLeft);
  team24.attachHandler("/RightR", handleRightR);
  team24.attachHandler("/LeftR", handleLeftR);
  team24.attachHandler("/Stop", handleStop);
  team24.attachHandler("/sliderDutyCyc?val=", handleSliderDutyCyc);
  team24.attachHandler("/sliderDutyCyc2?val=", handleSliderDutyCyc2);
  team24.attachHandler("/M_Def", handleMode_Def);
  team24.attachHandler("/M_Wall", handleMode_Wall);
  team24.attachHandler("/M_IR", handleMode_IR);
  team24.attachHandler("/M_Pol", handleMode_Pol);
}

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

void mForward(){
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
  Serial.println("Forward");
}

void mBackward(){
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH);
  Serial.println("Back");
}

void mLeft(){
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
  Serial.println("Left R");
}

void mRight(){
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, LOW); 
  Serial.println("Right R"); 
}

void mLeftR(){
  digitalWrite(MOTOR1_PIN1, LOW);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, LOW);
  Serial.println("Left R");
}

void mRightR(){
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, LOW);
  digitalWrite(MOTOR2_PIN1, LOW);
  digitalWrite(MOTOR2_PIN2, HIGH); 
  Serial.println("Right R"); 
}

void mStop(){
  digitalWrite(MOTOR1_PIN1, HIGH);
  digitalWrite(MOTOR1_PIN2, HIGH);
  digitalWrite(MOTOR2_PIN1, HIGH);
  digitalWrite(MOTOR2_PIN2, HIGH);
  Serial.println("Stop R");  
}

// Write duty to LEDC
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = LEDC_VALUE_MAX) {            
  uint32_t duty =  LEDC_RESOLUTION * min(value, valueMax) / valueMax;  
  ledcWrite(channel, duty);  
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


// WiFi Functions
void UdpSend(int x, int y){
  char udpBuffer[20];
  sprintf(udpBuffer, "%02d:%4d,%4d",teamNumber,x,y);   
  UDPTestServer.beginPacket(ipTarget, UDPPORT);
  UDPTestServer.println(udpBuffer);
  UDPTestServer.endPacket();
  Serial.println(udpBuffer);
}

void handleUDPServer() {
   const int UDP_PACKET_SIZE = 14; // can be up to 65535          
   uint8_t packetBuffer[UDP_PACKET_SIZE];

   int cb = UDPTestServer.parsePacket(); // if there is no message cb=0
   if (cb) {
      packetBuffer[13]=0; // null terminate string
      UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE);
      if (atoi((char *)packetBuffer) == 0) {
        police_x = atoi((char *)packetBuffer+3); // ##,####,#### 2nd indexed char
        police_y = atoi((char *)packetBuffer+8); // ##,####,#### 7th indexed char
        Serial.print("From Team ");
        Serial.println((char *)packetBuffer);
        Serial.println("Police Car data: ");
        Serial.println(police_x);
        Serial.println(police_y);
      }      
   }
}
// ESPNow
// callback when data is sent 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) Serial.println ("Success ");
  else Serial.println("Fail "); 
}

void setup_wifi(){
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP_STA); 
  IPAddress myIP(192,168,1,132);
  // Set WiFi to AP mode
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(myIP, IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  Serial.printf("Connected to %s on ", ssid); 
  Serial.println(myIP);  //Print ssid and IP address

  // Vive
  Serial.printf("Team  #%d", teamNumber); 
  UDPTestServer.begin(UDPPORT);
  vive1.begin();
  Serial.println("Vive trackers started");

  // ESPNow
  Serial.print("Sending MAC: "); Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESPnow init failed");
    ESP.restart();
  }
  esp_now_register_send_cb(OnDataSent); //optional if you want ack interrupt
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("ESPnow Pair failed");     // ERROR  should not happen
  }
}

void IRDetect() {
  // Iterate over each sensor. Assuming there are 2 sensors connected.
  for (int i = 0; i < 2; i++) {
    // Read the current value from the IR sensor
    unsigned long currentReading = analogRead(IRpins[i]);

    // Check if the current sensor reading is above the threshold and was previously below it
    // This indicates a rising edge, which is the start of a high pulse
    if (currentReading >= highThreshold && lastAnalogValue[i] < highThreshold) {
      // Calculate the duration of the previous pulse
      pulseDuration[i] = micros() - lastRisingEdge[i];
      // Record the time of this rising edge
      lastRisingEdge[i] = micros();
      // Mark that we're currently in a high pulse
      isHighPulse[i] = true;

      // Calculate the frequency based on the pulse duration
      frequency[i] = 1000000.0 / pulseDuration[i]; // Convert pulse duration to frequency
      // Print the frequency to the Serial Monitor for debugging
      Serial.print("Frequency of sensor ");
      Serial.print(i + 1);
      Serial.print(" is: ");
      Serial.print(frequency[i]);
      Serial.println(" Hz");
    }
    // Check if the current sensor reading is below the threshold and we were previously in a high pulse
    // This indicates a falling edge, which is the end of a high pulse
    else if (currentReading < highThreshold && isHighPulse[i]) {
      // Mark that the high pulse has ended
      isHighPulse[i] = false;
    }

    // Store the current reading to compare in the next iteration
    lastAnalogValue[i] = currentReading;
  }
}



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

// Vive Functions
void viveSend() {
  if (millis()-vive_ms>1000/FREQ) {
    vive_ms = millis();
    if (WiFi.status()==WL_CONNECTED) {
      neopixelWrite(RGBLED,255,255,255);  // full white
      UdpSend(vive_x, vive_y);
    }        
  }
  
  if (vive1.status() == VIVE_RECEIVING) {
    vive_x = vive1.xCoord();
    vive_y = vive1.yCoord();
    neopixelWrite(RGBLED, 0, vive_x/200, vive_y/200);  // blue to greenish
  }
  else {
    vive_x=0;
    vive_y=0; 
    switch (vive1.sync(5)) {
      break;
      case VIVE_SYNC_ONLY: // missing sweep pulses (signal weak)
        neopixelWrite(RGBLED,64,32,0);  // yellowish
      break;
      default:
      case VIVE_NO_SIGNAL: // nothing detected     
        neopixelWrite(RGBLED,128,0,0);  // red
    }
  }
  delay(20);
}

// Police Car Finding
int cal_norm() {
  handleUDPServer();  // Getting Police Car vive x y
  viveSend();     // Getting our robots vive x y
  // Update police xy
  int distance = pow((pow((police_x - vive_x), 2) + pow((police_y - vive_y), 2)), 0.5); // Norm distance
  int dist_x = pow(pow((police_x - vive_x), 2), 0.5);     // X distance
  int dist_y = pow(pow((police_y - vive_y), 2), 0.5);     // Y distance
  UdpSend(dist_x, dist_y);
  Serial.printf("Car Vive Position: %d, %d", vive_x, vive_y);
  Serial.printf("Cal Norm: %d", distance);
  return distance;
}

int rot_count = 0;

void align_robot() {
  int init_dist = cal_norm();
  int cur_dist;
  int find_dir = 1;   // If 1 => Go straight and check if distance decrease, 0 => turn

  while (find_dir == 1) {
    mBackward();
    delay(200);
    mStop();
    cur_dist = cal_norm();
    Serial.printf("Align Robot: %d", cur_dist);
    if (cur_dist < init_dist - 100) {
      Serial.println("Getting Closer");
      find_dir = 0;
      delay(500);
    }
    else if (rot_count < 3) {
      Serial.println("Still Finding...");
      mForward();
      delay(200);
      mLeftR();
      delay(10);
      mStop();
      delay(100);
      rot_count += 1;
    }
    else if (rot_count >= 3) {
      Serial.println("Still Finding...");
      mForward();
      delay(200);
      mLeftR();
      delay(10);
      mStop();
      delay(100);
      rot_count += 1;
      if (rot_count > 5) {
        rot_count = 0;
      }
    }
  }
}

void move_to_pol() {
  Serial.println("Get Vive Data...");
  int distance = cal_norm();
  if (distance > 200) {
    Serial.println("Aligning...");
    align_robot();
    Serial.println("Move toward Police Car");
    mBackward();
    delay(100);
    mStop();
    delay(500);

  }
  else if (distance < 200) {
  Serial.println("Pushing Police Car");
    mBackward();
  delay(1000);
  mStop();
    delay(100);
}
  else if (distance < 5000) {
    Serial.println("Out of bound, wait for Car Vive");
    delay(100);
  }
  else {
    mStop();
  }
}



void espSend() {
  uint8_t message[50]; // Max ESPnow packet is 250 byte data
  message[0]=2;
  message[1]=1;
  message[1]=3;
  // put some message together to send
  sprintf((char *) message, "Team 24 ESPNow" );
  
  if (esp_now_send(peer1.peer_addr, message, sizeof(message))==ESP_OK) 
    Serial.printf("Sent '%s' to %x:%x:%x:%x:%x:%x \n",message, peer1.peer_addr[0],peer1.peer_addr[1],peer1.peer_addr[2],peer1.peer_addr[3],peer1.peer_addr[4],peer1.peer_addr[5]);   
  else Serial.println("Send failed");
  
  // delay(500); // ESPNow max sending rate (with default speeds) is about 50Hz
}



void setup() {
  Serial.begin(115200);     //Set data rate in baud (bits/secs)
  Wire.begin(SDA_PIN, SCL_PIN);     // Set up i2C Ports
  // wait until serial port opens for native USB devices
  while (! Serial) { delay(1); }

  // Set up TOF pin
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setup_tof();
  // Set up the IR sensor pin
  pinMode(IRpins[0],INPUT);
  pinMode(IRpins[1],INPUT);
  // Set up encoder pin
  pinMode(ENCDPinMot1, INPUT);
  pinMode(ENCDPinMot2, INPUT);
  // Set up for motor
  setup_motor();
  // Set up Wifi
  setup_wifi();
  team24.begin();    // Server begins
  // Register URL Handlers
  setup_handler();
}


// int rot_ir = 0;

void loop() {
  team24.serve();  
  frequency[0] = 0;
  frequency[1] = 0;
  IRDetect();
  if (op_mode == 0) {
    // Default    
    encoder();
    PIDControl();
    ledcAnalogWrite(LEDC_CHANNEL1, u[0]); // motor1
    ledcAnalogWrite(LEDC_CHANNEL2, u[1]); // motor2
  }
  else if (op_mode == 1) {
    // Wall Following
    Serial.println("Wall Following...");
    read_dual_sensors();
    wall_following();
  }
  else if (op_mode == 2) {
    // IR Mode
    mLeftR();
    delay(110);
    mStop();
    if (abs(frequency[0]-550)<50){
      mForward();
      delay(900);
      mStop();
    }

  }
  else if (op_mode == 3) {
    // Police Car
    mStop();
    ledcAnalogWrite(LEDC_CHANNEL1, 1200); // motor1
    ledcAnalogWrite(LEDC_CHANNEL2, 1200); // motor2
    move_to_pol();
  } 

}

