// WiFi Configuration
const char* ssid     = "Team Pao";
const char* password = "987654321";
WiFiUDP UDPTestServer;
IPAddress ipTarget(192, 168, 1, 255); // 255 => broadcast
IPAddress myIP(192, 168, 1, 217); // change to your IP

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