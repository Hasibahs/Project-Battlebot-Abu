#ifndef GLOBALS_H
#define GLOBALS_H

#include <Servo.h>

extern const int MOTOR_RIGHT_FORWARD;
extern const int MOTOR_LEFT_FORWARD;
extern const int MOTOR_LEFT_BACKWARD;
extern const int MOTOR_RIGHT_BACKWARD;
extern const int trigPin;
extern const int echoPin;
extern const int servoPin;
extern const int motorSpeed;
extern const int turnDuration;
extern const int obstacleDistance;
extern Servo gripper;
extern bool initialPartFinished;


extern const int SENSOR_PINS[8];
extern int thresholds[8];
extern const int BASE_SPEED;
extern const int MAX_ADJUSTMENT;
extern const unsigned long BLACK_DETECTION_THRESHOLD;
extern unsigned long blackDetectionStartTime;
extern bool isDetectingBlack;
extern Servo gripperServo;

#endif
