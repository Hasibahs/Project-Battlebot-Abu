// Define the motor control pins
const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

// Define the threshold values for each sensor
const int thresholds[8] = {854, 833, 850, 860, 873, 870, 884, 894};

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set the motor control pins as output
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
}

void loop() {
  // Create an array to hold the sensor values
  int sensorValues[8];
  int digitalValues[8];
  int blackLineCount = 0;

  // Read the sensor values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
    digitalValues[i] = sensorValues[i] > thresholds[i] ? 1 : 0;

    // Count the number of sensors detecting the black line
    if (digitalValues[i] == 1) {
      blackLineCount++;
    }
  }

  // Print the sensor values and digital states to the serial monitor
  for (int i = 0; i < 8; i++) {
    Serial.print("Sensor A");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(sensorValues[i]);
    Serial.print(" (");
    Serial.print(digitalValues[i]);
    Serial.print(") ");
  }
  Serial.println();

  // Control the motors based on sensor values
  if (blackLineCount > 4) { // If more than half of the sensors detect the black line
    moveForward();
  } else {
    stop();
  }

  // Add a short delay to slow down the loop
  delay(100);
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void stop() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
