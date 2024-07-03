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

  // Read the sensor values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
    digitalValues[i] = sensorValues[i] > thresholds[i] ? 1 : 0;
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

  // Determine the line position and control the motors
  controlMotors(digitalValues);

  // Add a short delay to smooth out the loop
  delay(100);
}

void controlMotors(int* digitalValues) {
  int position = 0;
  int count = 0;

  // Calculate the position by summing the weighted sensor values
  for (int i = 0; i < 8; i++) {
    if (digitalValues[i] == 1) {
      position += (i - 3.5) * 2; // Weighted position: -7, -5, -3, -1, 1, 3, 5, 7
      count++;
    }
  }

  if (count == 0) {
    // If no line is detected, stop the robot
    stop();
  } else {
    // Calculate the average position
    position = position / count;

    // Check for sharp turns
    if (digitalValues[0] == 1 && digitalValues[1] == 1 && digitalValues[2] == 1 && digitalValues[3] == 1) {
      // Sharp left turn detected
      sharpTurnLeft();
    } else if (digitalValues[4] == 1 && digitalValues[5] == 1 && digitalValues[6] == 1 && digitalValues[7] == 1) {
      // Sharp right turn detected
      sharpTurnRight();
    } else if (position > 1) {
      // If the position is greater than 1, turn right slightly
      slightTurnRight();
    } else if (position < -1) {
      // If the position is less than -1, turn left slightly
      slightTurnLeft();
    } else {
      // If the position is between -1 and 1, move forward
      moveForward();
    }
  }
}

void moveForward() {
  analogWrite(MOTOR_LEFT_FORWARD, 200); // Adjust speed as needed
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 200); // Adjust speed as needed
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void slightTurnLeft() {
  analogWrite(MOTOR_LEFT_FORWARD, 150);  // Reduce speed for slight turn
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 200); // Keep the right motor speed constant
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void slightTurnRight() {
  analogWrite(MOTOR_LEFT_FORWARD, 200);  // Keep the left motor speed constant
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 150); // Reduce speed for slight turn
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void sharpTurnLeft() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);    // Stop the left motor
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 200); // Run the right motor at full speed
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void sharpTurnRight() {
  analogWrite(MOTOR_LEFT_FORWARD, 200);  // Run the left motor at full speed
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);   // Stop the right motor
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void stop() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}
