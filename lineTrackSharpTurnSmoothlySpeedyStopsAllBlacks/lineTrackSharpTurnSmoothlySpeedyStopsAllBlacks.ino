//motor control pins
const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

//the threshold values for each sensor
const int thresholds[8] = {854, 833, 850, 860, 873, 870, 884, 894};

//the base speed and the maximum adjustment
const int BASE_SPEED = 180; 
const int MAX_ADJUSTMENT = 100;

void setup() {
  Serial.begin(9600);

  //motor control pins as output
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
}

void loop() {
  int sensorValues[8];
  int digitalValues[8];

  // Reading the IR sensor values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
    digitalValues[i] = sensorValues[i] > thresholds[i] ? 1 : 0;
  }

  // IR debugging and show it in serial monitor
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

  // for smooth out the loop
  delay(100);
}

void controlMotors(int* digitalValues) {
  int position = 0;
  int count = 0;
  bool allBlack = true;

  // Calculate the position by weighted sensor
  for (int i = 0; i < 8; i++) {
    if (digitalValues[i] == 1) {
      position += (i - 3.5) * 2; // Weighted positions are: -7, -5, -3, -1, 1, 3, 5, 7
      count++;
    } else {
      allBlack = false; // If any sensor does not detect black, set allBlack to false
    }
  }

  if (allBlack || count == 0) {
    // If all sensors detect black or no line is detected, stop the robot
    stop();
  } else {
    // Calculate average position
    position = position / count;

    // Proportional control
    int adjustment = position * (MAX_ADJUSTMENT / 7);

    int leftSpeed = BASE_SPEED + adjustment;
    int rightSpeed = BASE_SPEED - adjustment;

    // Limit the speed values to the range [0, 255]
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    moveForward(leftSpeed, rightSpeed);
  }
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_FORWARD, leftSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, rightSpeed);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void sharpTurnLeft() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);    // Stop left motor
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, BASE_SPEED + MAX_ADJUSTMENT); // Run right motor at increased speed
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void sharpTurnRight() {
  analogWrite(MOTOR_LEFT_FORWARD, BASE_SPEED + MAX_ADJUSTMENT);  // Run left motor at increased speed
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);   // Stop right motor
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void stop() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}
