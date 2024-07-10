#include <Arduino.h>
#include "globals.h"
#include "second.h"
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel strip;

long getDistanceSecondPart() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

int readSensors() {
  int sensorValues = 0;
  for (int i = 0; i < 8; i++) {
    int sensorValue = analogRead(SENSOR_PINS[i]);
    if (sensorValue > thresholds[i]) {
      sensorValues |= (1 << i);
    }
  }
  return sensorValues;
}

int calculateError(int sensors) {
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
    return error / count;
  } else {
    return 0;
  }
}

void setMotors(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_FORWARD, max(0, leftSpeed));
  analogWrite(MOTOR_RIGHT_FORWARD, max(0, rightSpeed));
  analogWrite(MOTOR_LEFT_BACKWARD, max(0, -leftSpeed));
  analogWrite(MOTOR_RIGHT_BACKWARD, max(0, -rightSpeed));
}

void blinkAllLEDs() {
  static bool ledState = false;
  ledState = !ledState;
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, ledState ? strip.Color(255, 255, 255) : strip.Color(0, 0, 0));
  }
  strip.show();
}

void handleSecondPart() {
  static bool objectGrabbed = false;

  int sensors = readSensors();
  Serial.print("Sensors: ");
  for (int i = 0; i < 8; i++) {
    Serial.print((sensors >> i) & 1);
    Serial.print(" ");
  }
  Serial.println();
  
  if (sensors == 0b11111111) {
    if (!isDetectingBlack) {
      isDetectingBlack = true;
      blackDetectionStartTime = millis();
    }
    if (millis() - blackDetectionStartTime >= BLACK_DETECTION_THRESHOLD) {
      setMotors(0, 0);
      gripperServo.write(135);
      if (objectGrabbed) {
        gripper.write(135);  // Release the object at the end of line tracking
        objectGrabbed = false;
      }
      while (true) {
        blinkAllLEDs();
        delay(500); 
      }
      return;
    }
  } else {
    isDetectingBlack = false;
  }
  
  if (sensors == 0b00000000) {
    setMotors(0, 0);
    gripperServo.write(135);
  } else {
    int error = calculateError(sensors);
    int adjustment = error * MAX_ADJUSTMENT / 4;
    if (error < -1 || error > 1) {
      adjustment = error * MAX_ADJUSTMENT / 2;
    }
    int leftSpeed = BASE_SPEED + adjustment;
    int rightSpeed = BASE_SPEED - adjustment;
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);
    setMotors(leftSpeed, rightSpeed);
    
    // LED indication for turns
    if (leftSpeed < rightSpeed) {
      // Indicate left turn
      strip.setPixelColor(0, strip.Color(255, 0, 0)); 
      strip.setPixelColor(3, strip.Color(255, 0, 0)); 
      strip.setPixelColor(1, strip.Color(0, 0, 0));   
      strip.setPixelColor(2, strip.Color(0, 0, 0));   
    } else if (leftSpeed > rightSpeed) {
      // Indicate right turn
      strip.setPixelColor(1, strip.Color(0, 0, 255)); 
      strip.setPixelColor(2, strip.Color(0, 0, 255)); 
      strip.setPixelColor(0, strip.Color(0, 0, 0));   
      strip.setPixelColor(3, strip.Color(0, 0, 0));   
    } else {
      // Going straight, turn off LEDs
      strip.setPixelColor(0, strip.Color(0, 0, 0)); 
      strip.setPixelColor(1, strip.Color(0, 0, 0)); 
      strip.setPixelColor(2, strip.Color(0, 0, 0)); 
      strip.setPixelColor(3, strip.Color(0, 0, 0)); 
    }
    strip.show();
  }
  
  long distance = getDistanceSecondPart();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance <= 4 && !objectGrabbed) {
    gripper.write(70);  // Grab the object
    objectGrabbed = true;
  }
  
  delay(50);
}
