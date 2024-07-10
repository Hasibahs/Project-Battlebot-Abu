#include <Arduino.h>
#include "globals.h"
#include "setup.h"
#include "initial.h"
#include "second.h"

void setup() {
  setupPins();
  setupServos();
  Serial.begin(9600);
}

void loop() {
  if (!initialPartFinished) {
    handleInitialPart();
  } else {
    handleSecondPart();
  }
}
