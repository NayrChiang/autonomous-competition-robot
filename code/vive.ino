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

// Vive Object Finding
// Called "Police Car" in the project
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