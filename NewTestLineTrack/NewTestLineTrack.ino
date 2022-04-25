#include <Adafruit_SSD1306.h>

//#include <splash.h>

#include <Arduino.h>
#include <analogWrite.h>
#include <Wire.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

volatile int a;
volatile int b;
volatile int S3;
volatile int M2;
volatile int js;
volatile int touch;
boolean runOrder;

int rightSensor = 39;
int leftSensor = 34;

int RB=16;
int RF=17;
int LF=5;
int LB=18;

void mainLoop() {
  switch (a) {
   case 0:
    analogWrite(17, js);
    analogWrite(16, 0);
    analogWrite(5, js);
    analogWrite(18, 0);
    break;
   case 1://turn right
    analogWrite(17, js);
    analogWrite(16, (js + 60));
    analogWrite(5, (js + 60));
    analogWrite(18, js);
    break;
   case -1://turn left
    analogWrite(17, (js + 60));
    analogWrite(16, js);
    analogWrite(5, js);
    analogWrite(18, (js + 60));
    break;
   case 2://turn right the
    analogWrite(5, (js + 60));
    analogWrite(16, (js + 60));
    analogWrite(17, (js - 60));
    analogWrite(18, (js - 60));
    break;
   case -2://turn left
    analogWrite(15, (js - 60));
    analogWrite(16, (js - 60));
    analogWrite(17, (js + 60));
    analogWrite(18, (js + 60));
    break;
  }
}

void Stop() {
  analogWrite(17, 0);
  analogWrite(18, 0);
  analogWrite(15, 180);
  analogWrite(14, 180);
}

void measure() {
  if (analogRead(leftSensor) > touch && analogRead(rightSensor) > touch) {
    a = 0;
    b = 10;
  } else if (analogRead(leftSensor) < touch && analogRead(rightSensor) > touch) {
    a = -1;
    b = b - 1;
  } else if ((analogRead(leftSensor) < touch && analogRead(rightSensor) < touch) && b < 10) {
    a = -2;
  } else if (analogRead(leftSensor) > touch && analogRead(rightSensor) < touch) {
    a = 1;
    b = b + 1;
  } else if ((analogRead(leftSensor) < touch && analogRead(rightSensor) < touch) && b > 10) {
    a = 2;
  }
}

void disp() {
  int x = 8;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(a);
  display.setCursor(0,x);
  display.print(b);
  display.setCursor(0,x*2);
  display.print(analogRead(rightSensor));
  display.setCursor(0,x*3);
  display.print(analogRead(leftSensor));
  display.display();
  delay(100);
}



void setup(){
  a = 0;
  b = 0;
  S3 = 0;
  M2 = 0;
  js = 180;
  touch = 900;
  runOrder = true;
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop(){
  if (runOrder) {
    for(int i=0;i<2;i++)
    {
       S3 = S3 + 1;
       M2 = (long) (S3) % (long) (3);
    }
  }
  switch (M2) {
   case 0:
    Stop();
    break;
   case 1:
    disp();
    break;
   case 2:
    measure();
    mainLoop();
    Serial.println(a);
    Serial.println(b);
    Serial.println(S3);
    Serial.println(M2);
    break;
  } 
}
