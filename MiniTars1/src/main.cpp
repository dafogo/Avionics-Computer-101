#include <Arduino.h>

#include "StateMachine.h"

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
  delay(100);

}