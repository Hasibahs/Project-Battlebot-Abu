const int NUM_SENSORS = 8;
const int SENSOR_PINS[NUM_SENSORS] = {A6, A7, A0, A1, A2, A3, A5, A4}; // Corrected order based on your readings
int sensorValues[NUM_SENSORS];
const int THRESHOLD = 700; // Updated threshold value based on new data

const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

const int MOTOR_SPEED = 200; // Adjust motor speed (0-255) for line-following

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
}

void loop() {
  readSensors();
  controlMotors();
  delay(10); // Reduced delay for more frequent adjustments
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
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sensorValues[i]);
  }
  Serial.println();
}

void controlMotors() {
  bool onLine = false;
  int sum = 0;
  int count = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] > THRESHOLD) { // Adjusted logic to match your data
      onLine = true;
      sum += sensorValues[i];
      count++;
    }
  }

  if (onLine && count > 0) {
    int averageValue = sum / count;
    Serial.print("Average Black Line Sensor Value: ");
    Serial.println(averageValue);

    // Determine the position of the black line and adjust motors accordingly
    if (sensorValues[2] > THRESHOLD && sensorValues[3] > THRESHOLD && sensorValues[4] > THRESHOLD && sensorValues[5] > THRESHOLD) {
      moveForward(); // All middle sensors on the line
    } else if (sensorValues[0] > THRESHOLD || sensorValues[1] > THRESHOLD) {
      turnLeft(); // Left sensors on the line
    } else if (sensorValues[6] > THRESHOLD || sensorValues[7] > THRESHOLD) {
      turnRight(); // Right sensors on the line
    } else {
      stop();
    }
  } else {
    stop();
  }
}

void moveForward() {
  analogWrite(MOTOR_LEFT_FORWARD, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_FORWARD, MOTOR_SPEED);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Moving Forward");
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  analogWrite(MOTOR_RIGHT_FORWARD, MOTOR_SPEED);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Turning Left");
}

void turnRight() {
  analogWrite(MOTOR_LEFT_FORWARD, MOTOR_SPEED);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Turning Right");
}

void stop() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Stopped");
}
