#include <Arduino.h>
#include "subsystems\TARSAccelerometer\TARSAccelerometer.h"
#include "subsystems\TARSbarometer\TARSbarometer.h"

#include "StateMachine.h"

StateMachine sm;

void setup() {
  Serial.begin(115200);
  sm.setup();
}


void loop() {
  sm.stateAction();
  sm.stateTransition();
  delay(100);

}