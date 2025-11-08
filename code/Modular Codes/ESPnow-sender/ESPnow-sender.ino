/**
 * Simple sender  to only a receiver ESPNOW with known MAC hard coded to MAC_RECV
*/

#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 0                  // channel can be 1 to 14, channel 0 means current channel.  
// #define MsAC_RECV  {0xa0,0x76,0x4e,0x19,0xea,0x2c} // receiver MAC address (last digit should be even for STA)
#define MAC_RECV  {0x68,0x67,0x25,0x82,0x3F,0x8C}
esp_now_peer_info_t peer1 = 
{
  .peer_addr = MAC_RECV, 
  .channel = CHANNEL,
  .encrypt = false,
};

// callback when data is sent 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) Serial.println ("Success ");
  else Serial.println("Fail "); 
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  
  Serial.print("Sending MAC: "); Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("init failed");
    ESP.restart();
  }
  
  esp_now_register_send_cb(OnDataSent); //optional if you want ack interrupt
    
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("Pair failed");     // ERROR  should not happen
  }
}

void loop() {
  static int count;
  uint8_t message[200]; // Max ESPnow packet is 250 byte data
  message[0]=2;
  message[1]=1;
  message[1]=3;
  // put some message together to send
  sprintf((char *) message, "sender team 24");
  
  if (esp_now_send(peer1.peer_addr, message, sizeof(message))==ESP_OK) 
    Serial.printf("Sent '%s' to %x:%x:%x:%x:%x:%x \n",message, peer1.peer_addr[0],peer1.peer_addr[1],peer1.peer_addr[2],peer1.peer_addr[3],peer1.peer_addr[4],peer1.peer_addr[5]);   
  else Serial.println("Send failed");
  
  delay(500); // ESPNow max sending rate (with default speeds) is about 50Hz
   
}
