// IR Setting
const int IRpins[] = {34,35};
int isHighPulse[] = {0,0};
unsigned long pulseDuration[] = {0,0};
float frequency[] = {0,0};
unsigned long lastAnalogValue[] = {0, 0};
int highThreshold = 2500; // Max 4096

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
  