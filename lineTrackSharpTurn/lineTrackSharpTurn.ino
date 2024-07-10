// Define the motor control pins
const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

// Define the IR sensor pins
const int SENSOR_PINS[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

// Define the threshold values for each sensor
int thresholds[8] = {693, 655, 686, 721, 741, 736, 758, 791};

// Define the base speed and the maximum adjustment
const int BASE_SPEED = 200; // Increased base speed for faster movement
const int MAX_ADJUSTMENT = 200; // Increased maximum adjustment for more aggressive turns

// Define the time threshold for stopping (in milliseconds)
const unsigned long BLACK_DETECTION_THRESHOLD = 300; // 300 milliseconds

// Variables to track black detection duration
unsigned long blackDetectionStartTime = 0;
bool isDetectingBlack = false;

// Function to read sensor values
int readSensors() {
  int sensorValues = 0;
  for (int i = 0; i < 8; i++) {
    int sensorValue = analogRead(SENSOR_PINS[i]);
    if (sensorValue > thresholds[i]) {
      sensorValues |= (1 << i); // Set the corresponding bit if the sensor detects black
    }
  }
  return sensorValues;
}

// Function to calculate the error value
int calculateError(int sensors) {
  // Assign weights to each sensor position
  int weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
  int error = 0;
  int count = 0;
  
  for (int i = 0; i < 8; i++) {
    if ((sensors >> i) & 1) {
      error += weights[i];
      count++;
    }
  }

  if (count > 0) {
    return error / count; // Average error
  } else {
    return 0; // No error if no sensors are detecting black
  }
}

// Function to control the motors
void setMotors(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_FORWARD, max(0, leftSpeed));
  analogWrite(MOTOR_RIGHT_FORWARD, max(0, rightSpeed));
  analogWrite(MOTOR_LEFT_BACKWARD, max(0, -leftSpeed));
  analogWrite(MOTOR_RIGHT_BACKWARD, max(0, -rightSpeed));
}

void setup() {
  // Set the motor control pins as outputs
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  int sensors = readSensors();

  // Debugging: Print sensor values
  Serial.print("Sensors: ");
  for (int i = 0; i < 8; i++) {
    Serial.print((sensors >> i) & 1);
    Serial.print(" ");
  }
  Serial.println();

  if (sensors == 0b11111111) {
    // All sensors detect black
    if (!isDetectingBlack) {
      isDetectingBlack = true;
      blackDetectionStartTime = millis();
    }
    
    if (millis() - blackDetectionStartTime >= BLACK_DETECTION_THRESHOLD) {
      // Stop if detecting black for more than the threshold duration
      setMotors(0, 0);
      return;
    }
  } else {
    // Reset black detection tracking if not all sensors detect black
    isDetectingBlack = false;
  }

  if (sensors == 0b00000000) {
    // All sensors detect white, stop
    setMotors(0, 0);
  } else {
    int error = calculateError(sensors);
    int adjustment = error * MAX_ADJUSTMENT / 4; // Scale the error to the maximum adjustment

    // Make the turns more aggressive if the error is significant
    if (error < -1 || error > 1) {
      adjustment = error * MAX_ADJUSTMENT / 2;
    }

    int leftSpeed = BASE_SPEED + adjustment;
    int rightSpeed = BASE_SPEED - adjustment;

    // Ensure speeds are within the valid range
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    setMotors(leftSpeed, rightSpeed);
  }

  delay(50); // Small delay to stabilize the robot
}
