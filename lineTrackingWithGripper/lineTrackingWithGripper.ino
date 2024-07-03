#include <Servo.h>

// motor pins
const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

// threshold values for each sensor
const int thresholds[8] = {854, 833, 850, 860, 873, 870, 884, 894};

// base speed and maximum adjustment
const int BASE_SPEED = 180; // Moderate increase in base speed
const int MAX_ADJUSTMENT = 100; // Moderate increase in maximum adjustment

// ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 8;

// gripper pin
const int servoPin = 11;

long duration;
int distance;

Servo myServo;

void setup() {
  Serial.begin(9600);

  // motor pins as output
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(130); // Open the gripper initially to 130 degrees
}

void loop() {
  int sensorValues[8];
  int digitalValues[8];

  // Read the IR sensors values from A0 to A7
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(A0 + i);
    digitalValues[i] = sensorValues[i] > thresholds[i] ? 1 : 0;
  }

  // Debugging tool: Print the sensor values and digital states to the serial monitor
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

  // Check for objects with ultrasonic sensor
  checkForObject(digitalValues);

  delay(100);
}

void controlMotors(int* digitalValues) {
  int position = 0;
  int count = 0;
  bool allBlack = true;

  // Calculate the position by summing the weighted sensor values
  for (int i = 0; i < 8; i++) {
    if (digitalValues[i] == 1) {
      position += (i - 3.5) * 2; // Weighted position: -7, -5, -3, -1, 1, 3, 5, 7
      count++;
    } else {
      allBlack = false; // If any sensor does not detect black, set allBlack to false
    }
  }

  if (allBlack || count == 0) {
    // If all sensors detect black or no line is detected, stop the robot and open the gripper
    stop();
    myServo.write(130); // Open the gripper when all sensors detect black
  } else {
    // Calculate the average position
    position = position / count;

    // Proportional control
    int adjustment = position * (MAX_ADJUSTMENT / 7); 

    int leftSpeed = BASE_SPEED + adjustment;
    int rightSpeed = BASE_SPEED - adjustment;

    // the speed values to the range [0, 255]
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    moveForward(leftSpeed, rightSpeed);
  }
}

void checkForObject(int* digitalValues) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // debugger: Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // distance 8 cm
  if (distance <= 8) {
    Serial.println("Object detected within 8 cm. Closing gripper.");
    closeGripperFaster(); // Close the gripper faster
  }
}

void closeGripperFaster() {
  int currentAngle = myServo.read();
  while (currentAngle > 0) {
    currentAngle -= 4;
    myServo.write(currentAngle);
    delay(5); //faster closing
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
