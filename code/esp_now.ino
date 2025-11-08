// ESPNow Parameters
#define CHANNEL 0                  // channel can be 1 to 14, channel 0 means current channel.  
#define MAC_RECV  {0x60,0x55,0xF9,0x57,0x47,0x4C}
esp_now_peer_info_t peer1 = 
{
  .peer_addr = MAC_RECV, 
  .channel = CHANNEL,
  .encrypt = false,
};

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
