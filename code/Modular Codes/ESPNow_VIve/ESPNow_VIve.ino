/**
 * Simple sender  to only a receiver ESPNOW with known MAC hard coded to MAC_RECV
*/

#include <esp_now.h>
#include <WiFi.h>
#include "html510.h"

HTML510Server team24(80);    // Server Object for port 80 initialation the object with my name

const char* ssid_ap     = "Team241";
const char* password_ap = "12121212";
const char* ssid     = "TP-Link_E0C8";
const char* password = "52665134";

#define CHANNEL 0                  // channel can be 1 to 14, channel 0 means current channel.  
// Ryan Mac Address
#define MAC_RECV  {0x68,0x67,0x25,0x82,0x3F,0x8C} // receiver MAC address (last digit should be even for STA)
// #define MAC_RECV  {0xA0,0x76,0x4E,0x19,0xEA,0x2C}
esp_now_peer_info_t peer1 = 
{
  .peer_addr = MAC_RECV, 
  .channel = CHANNEL,
  .encrypt = false,
};

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

    // Mouse up currently doesnt work
    document.addEventListener('mouseup', function() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/Stop", true);
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

void setup_handler(){
  team24.attachHandler("/", handleRoot);             // For Content
}

// callback when data is sent 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) Serial.println ("Success");
  else Serial.println("Fail"); 
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA); 

  IPAddress myIP(192, 168, 1, 132);
  WiFi.softAP(ssid_ap, password_ap);
  WiFi.softAPConfig(myIP, IPAddress(192,168,1,1), IPAddress(255,255,255,0));
  Serial.printf("Connected to %s on ", ssid_ap); 
  Serial.println(myIP);  //Print ssid and IP address

  
  
  // WiFi.begin(ssid, password);
  // Serial.print("Connecting to ");  Serial.println(ssid);
  // while(WiFi.status()!=WL_CONNECTED){
  //   delay(100);   Serial.print(".");
  // }
  // Serial.print("WiFi connected to "); Serial.println(WiFi.localIP());
  // WiFi.config(IPAddress(192, 168, 1, 217), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0)); 
  

  Serial.print("Sending MAC: "); Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("init failed");
    ESP.restart();
  }
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("Pair failed");     // ERROR  should not happen
  }
  esp_now_register_send_cb(OnDataSent); //optional if you want ack interrupt
  
  team24.begin();    // Server begins
  // Register URL Handlers
  setup_handler();

}

void loop() {
  

  static int count;
  uint8_t message[50]; // Max ESPnow packet is 250 byte data
  message[0]=2;
  message[1]=1;
  message[1]=3;
  // put some message together to send
  sprintf((char *) message, "sender team 24");
  
  if (esp_now_send(peer1.peer_addr, message, sizeof(message))==ESP_OK) 
    Serial.printf("Sent '%s' to %x:%x:%x:%x:%x:%x \n",message, peer1.peer_addr[0],peer1.peer_addr[1],peer1.peer_addr[2],peer1.peer_addr[3],peer1.peer_addr[4],peer1.peer_addr[5]);   
  else Serial.println("Send failed");
  
  delay(500); // ESPNow max sending rate (with default speeds) is about 50Hz
  // team24.serve();
}
