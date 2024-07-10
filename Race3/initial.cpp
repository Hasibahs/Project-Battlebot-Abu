#include <Arduino.h>
#include "globals.h"
#include "initial.h"

void goForward() {
  analogWrite(MOTOR_RIGHT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void turnLeft() {
  analogWrite(MOTOR_RIGHT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void stopMotors() {
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}

void controlGripper() {
  static bool gripperMovingUp = true;
  static int pos = 55;

  if (gripperMovingUp) {
    pos += 1;
    if (pos >= 135) {
      gripperMovingUp = false;
    }
  } else {
    pos -= 1;
    if (pos <= 55) {
      gripperMovingUp = true;
    }
  }
  gripper.write(pos);
  delay(15); 
}

void handleInitialPart() {
  int distance = getDistance();
  
  while (distance <= obstacleDistance) {
    controlGripper(); 
    distance = getDistance(); 
  }

  stopMotors(); 
  
  goForward();
  delay(1500);
  turnLeft();
  delay(turnDuration);
  gripper.write(135);
  initialPartFinished = true;

  stopMotors();
  delay(500);
}
