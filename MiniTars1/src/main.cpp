#include <Arduino.h>
#include "subsystems\TARSAccelerometer\TARSAccelerometer.h"
#include "subsystems\TARSbarometer\TARSbarometer.h"

int statusLED = 2;
IMU_Accelerometer IMU;

void setup() {
  Serial.begin(115200);
  pinMode(statusLED, OUTPUT);
  delay(3000);
  IMU.IMU_Setup();
}


void loop() {

  for (int downCounter = 1 ; IMU.flightStatus() == 0; downCounter++){
    delay(100);
    if (downCounter == 6) {
      Serial.println("Abriendo Paracaídas");
      digitalWrite(statusLED, HIGH);
      delay(3000);
      digitalWrite(statusLED, LOW);
    }
  }
  delay(100);

}