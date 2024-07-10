#include <Arduino.h>
#include "globals.h"
#include "setup.h"

void setupPins() {
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void setupServos() {
  gripper.attach(servoPin);
  gripper.write(135); // neutral position
}
