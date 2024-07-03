void setup() {
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Create an array to hold the sensor values
  int sensorValues[8];

  // Read the sensor values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
  }

  // Print the sensor values to the serial monitor
  for (int i = 0; i < 8; i++) {
    Serial.print("A");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Add a short delay to slow down the loop
  delay(500);
}
