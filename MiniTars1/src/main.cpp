#include <Arduino.h>
#include "subsystems\TARSAccelerometer\TARSAccelerometer.h"
#include "subsystems\TARSbarometer\TARSbarometer.h"

int statusLED = 2;
IMU_Accelerometer IMU;

void setup() {
  Serial.begin(115200);
  // pinMode(13, OUTPUT);
  // pinMode(12, INPUT);
  delay(3000);
  IMU.IMU_Setup();
}


void loop() {
  // float maxAccelMag = 0;
  float AccelerationMag = IMU.getAccelVector();
  Serial.println("Acceleration = " + String(AccelerationMag) + "m/s^2");
  // while (AccelerationMag > maxAccelMag - 10) {

  // }
  // Serial.println("\n");
  delay(1000);
}