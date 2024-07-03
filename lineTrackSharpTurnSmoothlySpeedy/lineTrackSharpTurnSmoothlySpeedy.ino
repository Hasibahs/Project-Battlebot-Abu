#include <Servo.h>

// Motor control pins
const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

// Sensor pins and thresholds
const int trigPin = 9;
const int echoPin = 8;
const int servoPin = 11;
const int thresholds[8] = {854, 833, 850, 860, 873, 870, 884, 894};

// Speed settings
const int BASE_SPEED = 180;
const int MAX_ADJUSTMENT = 100;

// Variables for ultrasonic sensor and distance
long duration;
int distance;

// Servo object
Servo myServo;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Motor control pins as outputs
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach servo motor
  myServo.attach(servoPin);
  myServo.write(130); // Open the gripper initially to 130 degrees
}

void loop() {
  // Array to hold sensor values
  int sensorValues[8];
  int digitalValues[8];

  // Read the sensor values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
    digitalValues[i] = sensorValues[i] > thresholds[i] ? 1 : 0;
  }

  // Print sensor values and digital states to serial monitor
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

  // Control motors based on sensor values
  controlMotors(digitalValues);

  // Check for objects with ultrasonic sensor
  checkForObject();

  // Short delay to smooth out the loop
  delay(100);
}

void controlMotors(int* digitalValues) {
  int position = 0;
  int count = 0;

  // Calculate position by summing weighted sensor values
  for (int i = 0; i < 8; i++) {
    if (digitalValues[i] == 1) {
      position += (i - 3.5) * 2; // Weighted position: -7, -5, -3, -1, 1, 3, 5, 7
      count++;
    }
  }

  if (count == 0) {
    // Stop the robot if no line is detected
    stop();
  } else {
    // Calculate average position
    position = position / count;

    // Proportional control
    int adjustment = position * (MAX_ADJUSTMENT / 7); // Scale adjustment

    int leftSpeed = BASE_SPEED + adjustment;
    int rightSpeed = BASE_SPEED - adjustment;

    // Limit speed values to range [0, 255]
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    moveForward(leftSpeed, rightSpeed);
  }
}

void checkForObject() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check if distance is less than or equal to 8 cm
  if (distance <= 8) {
    Serial.println("Object detected within 8 cm. Closing gripper.");
    closeGripperFaster(); // Close the gripper faster
  } else {
    myServo.write(130); // Open the gripper to 130 degrees
  }
}

void closeGripperFaster() {
  int currentAngle = myServo.read();
  while (currentAngle > 0) {
    currentAngle -= 2; // Move faster by decrementing by 2
    myServo.write(currentAngle);
    delay(10); // Adjust the delay for faster closing
  }
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_FORWARD, leftSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, rightSpeed);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void stop() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}
