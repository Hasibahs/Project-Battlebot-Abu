#include <Servo.h>

Servo gripperServo;

const int GRIPPER_PIN = 11;

void setup() {
  gripperServo.attach(GRIPPER_PIN);
  Serial.begin(9600);
}

void loop() {
  // Test the gripper by opening and closing
  gripperServo.write(10);  // Open position
  delay(1000);
  gripperServo.write(90); // Mid position
  delay(1000);
  gripperServo.write(160); // Close position
  delay(1000);
}
