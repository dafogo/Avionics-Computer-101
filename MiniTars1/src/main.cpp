#include <Arduino.h>
#include "subsystems\TARSAccelerometer\TARSAccelerometer.h"
#include "subsystems\TARSbarometer\TARSbarometer.h"

void setup(void) {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  Serial.println("SI JALÓ");
  // Serial.println("Adafruit LSM6DSOX test!");
}

void loop() {
  IMU_Accelerometer IMU;
  while (1) {
    Serial.println("Displacement: " + String(IMU.x_displacement) + " m");
    while (digitalRead(12)){
      IMU.sumDisplacement();
    }
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }
}