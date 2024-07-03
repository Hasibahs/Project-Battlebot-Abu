#include <Servo.h>

Servo gripperServo;

const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

const int GRIPPER_PIN = 11;
const int ECHO_PIN = 13;
const int TRIG_PIN = 12;

void setup() {
  Serial.begin(9600);
  
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  gripperServo.attach(GRIPPER_PIN);
}

void loop() {
  // Motor and gripper test sequence
  moveForward();
  delay(2000);
  moveBackward();
  delay(2000);
  turnLeft();
  delay(2000);
  turnRight();
  delay(2000);

  // Gripper control
  gripperServo.write(0);  // Open position
  delay(1000);
  gripperServo.write(90); // Mid position
  delay(1000);
  gripperServo.write(180); // Close position
  delay(1000);

  // Ultrasonic sensor reading
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

long measureDistance() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) * 0.0343;
  return distance;
}
