const int NUM_SENSORS = 8;
const int SENSOR_PINS[NUM_SENSORS] = {A7, A6, A1, A0, A3, A2, A4, A5}; // Corrected order based on sensor readings
int sensorValues[NUM_SENSORS];

void setup() {
  Serial.begin(9600);
  // Analog pins are initialized by default
}

void loop() {
  readSensors();
  delay(1000); // Slow down the loop for readability
}

void readSensors() {
  // Read analog sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorValues[i] = analogRead(SENSOR_PINS[i]);
  }

  // Print sensor values for debugging
  Serial.println("Sensor Readings:");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValues[i]);
  }
  Serial.println();
}
