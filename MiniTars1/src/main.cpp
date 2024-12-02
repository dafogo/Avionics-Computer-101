#include <Arduino.h>

#include "state_machine/StateMachine.h"

StateMachine sm;

void setup() {
  Serial.begin(115200);
  sm.setup();
}


void loop() {
  sm.resetControlVariables();
  sm.getState();
  sm.stateAction();
  sm.stateTransition();
  Serial.println(sm.getState(), HEX);

  delay(100);

}