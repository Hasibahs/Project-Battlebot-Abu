#include <Arduino.h>
#include "globals.h"

const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;
const int trigPin = 9;
const int echoPin = 8;
const int servoPin = 11;
const int motorSpeed = 200;
const int turnDuration = 500;
const int obstacleDistance = 30;
Servo gripper;
bool initialPartFinished = false;

const int SENSOR_PINS[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int thresholds[8] = {693, 655, 686, 721, 741, 736, 758, 791};
const int BASE_SPEED = 200;
const int MAX_ADJUSTMENT = 200;
const unsigned long BLACK_DETECTION_THRESHOLD = 150;
unsigned long blackDetectionStartTime = 0;
bool isDetectingBlack = false;
Servo gripperServo;
