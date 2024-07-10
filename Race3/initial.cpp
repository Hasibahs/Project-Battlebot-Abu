#include <Arduino.h>
#include "globals.h"
#include "initial.h"
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;

void goForward() {
  analogWrite(MOTOR_RIGHT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  // Turn off LEDs
  strip.setPixelColor(0, strip.Color(0, 0, 0)); // Left back off
  strip.setPixelColor(1, strip.Color(0, 0, 0)); // Right back off
  strip.setPixelColor(2, strip.Color(0, 0, 0)); // Right front off
  strip.setPixelColor(3, strip.Color(0, 0, 0)); // Left front off
  strip.show();
}

void turnLeft() {
  analogWrite(MOTOR_RIGHT_FORWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_BACKWARD, motorSpeed);
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  // Indicate left turn
  strip.setPixelColor(0, strip.Color(255, 0, 0)); 
  strip.setPixelColor(3, strip.Color(255, 0, 0)); 
  strip.setPixelColor(1, strip.Color(0, 0, 0));   
  strip.setPixelColor(2, strip.Color(0, 0, 0));   
  strip.show();
}

void turnRight() {
  analogWrite(MOTOR_LEFT_FORWARD, motorSpeed);
  analogWrite(MOTOR_RIGHT_BACKWARD, motorSpeed);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  // Indicate right turn
  strip.setPixelColor(1, strip.Color(0, 0, 255)); 
  strip.setPixelColor(2, strip.Color(0, 0, 255)); 
  strip.setPixelColor(0, strip.Color(0, 0, 0));   
  strip.setPixelColor(3, strip.Color(0, 0, 0));   
  strip.show();
}

void stopMotors() {
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  // Turn off LEDs
  strip.setPixelColor(0, strip.Color(0, 0, 0)); 
  strip.setPixelColor(1, strip.Color(0, 0, 0)); 
  strip.setPixelColor(2, strip.Color(0, 0, 0)); 
  strip.setPixelColor(3, strip.Color(0, 0, 0)); 
  strip.show();
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
  
  // Blink LEDs
  static bool ledState = false;
  ledState = !ledState;
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, ledState ? strip.Color(255, 255, 255) : strip.Color(0, 0, 0));
  }
  strip.show();
}

void handleInitialPart() {
  int distance = getDistance();

  // Continuously check the distance and control the gripper
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
