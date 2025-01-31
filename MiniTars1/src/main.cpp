#include <Arduino.h>

#include "subsystems/TARSSD.h"

void setup() {
  Serial.begin(115200);
}


void loop() {
  // 4 registers per second
  // imu read
  // baro read
  // save in flash //TODO: actualizar flash (initial values = iter de vuelos, h init, current_state, last timestamp)
  

  //
}