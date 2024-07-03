const int NUM_SENSORS = 8;
const int SENSOR_PINS[NUM_SENSORS] = {A6, A7, A0, A1, A2, A3, A5, A4}; // Update with your pin order
int sensorValues[NUM_SENSORS];

void setup() {
  Serial.begin(9600);
}

void loop() {
  readSensors();
  delay(1000); // Slow down the loop for readability
}

void readSensors() {
  // Read sensor values
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(SENSOR_PINS[i]);
  }

  // Print sensor values for debugging
  Serial.println("Sensor Readings:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(SENSOR_PINS[i]);
    Serial.print(": ");
    Serial.println(sensorValues[i]);
  }
  Serial.println();
}
