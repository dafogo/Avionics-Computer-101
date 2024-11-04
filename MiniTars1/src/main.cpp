#include <Arduino.h>
#include "subsystems\TARSAccelerometer\TARSAccelerometer.h"
#include "subsystems\TARSbarometer\TARSbarometer.h"

int statusLED = 2;
IMU_Accelerometer IMU;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  // pinMode(12, INPUT);
  delay(3000);
  IMU.IMU_Setup();
}


void loop() {
  // float maxAccelMag = 0;
  // float AccelerationMag = IMU.getAccelVector();
  // Serial.println("Acceleration = " + String(AccelerationMag) + "m/s^2");
  for (int downCounter = 1 ; IMU.flightStatus() == 0; downCounter++){
    delay(100);
    if (downCounter == 6) {
      Serial.println("Abriendo Paracaídas");
      digitalWrite(2, HIGH);
      delay(3000);
      digitalWrite(2, LOW);
    }
  }
  // while (AccelerationMag > maxAccelMag - 10) {

  // }
  // Serial.println("\n");
  delay(100);
}