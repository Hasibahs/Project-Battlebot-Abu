const int MOTOR_RIGHT_FORWARD = 3;
const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 6;
const int MOTOR_RIGHT_BACKWARD = 4;

const int MOTOR_SPEED = 200; // Adjust motor speed (0-255)

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
}

void loop() {
  moveForward();
  delay(2000); // Move forward for 2 seconds

  stop();
  delay(1000); // Stop for 1 second

  moveBackward();
  delay(2000); // Move backward for 2 seconds

  stop();
  delay(1000); // Stop for 1 second
}

void moveForward() {
  analogWrite(MOTOR_LEFT_FORWARD, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_FORWARD, MOTOR_SPEED);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Moving Forward");
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  analogWrite(MOTOR_LEFT_BACKWARD, MOTOR_SPEED);
  analogWrite(MOTOR_RIGHT_BACKWARD, MOTOR_SPEED);
  Serial.println("Moving Backward");
}

void stop() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("Stopped");
}
