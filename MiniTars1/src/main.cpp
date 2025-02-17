#include <Arduino.h>

#include "state_machine/StateMachine.h"

StateMachine sm;

void setup() {
  Serial.begin(115200);
  Serial.println("The code starts...");

  sm.setup();
}


void loop() {
  sm.stateAction();
  sm.stateTransition();
  sm.periodicRegister(1);
  // 4 registers per second
  // imu read
  // baro read
  // save in flash //TODO: actualizar flash (initial values = iter de vuelos, h init, current_state, last timestamp)


  //
}