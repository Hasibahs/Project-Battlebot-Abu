#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "globals.h"
#include "setup.h"
#include "initial.h"
#include "second.h"

void setup() {
  setupPins();
  setupServos();
  Serial.begin(9600);

  // Initialize the NeoPixel library
  strip.begin();
  // Set all pixels to 'off'
  strip.show();
}

void loop() {
  if (!initialPartFinished) {
    handleInitialPart();
  } else {
    handleSecondPart();
  }
}
