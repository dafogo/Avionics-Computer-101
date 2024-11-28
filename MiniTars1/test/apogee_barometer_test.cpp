/*
 * @file apogee_barometer_test.cpp
 * @date 11/27/2024
 * @author Dana E. Torres Estrada
 * 
 * @brief Testing Apogee Detection with only the barometer.
 */

#include <Arduino.h>
#include "subsystems/TARSBarometer.h"

TARSBarometer barometer;
float lastHeight = 0.0; // h1
float currentHeight = 0.0; // h2
float deltaHeight = 0.0;

int freeFallCounter = 0;

void setup() {
    // Start the serial communication for debugging
    Serial.begin(115200);
    while (!Serial);

    // Initialize the BMP390 sensor
    if (!barometer.begin()) {
        while (1); // Halt execution if the sensor fails to initialize
    }
}

void loop(){

    currentHeight = barometer.calculateAltitude();
    deltaHeight = currentHeight - lastHeight;

    Serial.println(currentHeight);

    lastHeight = currentHeight;

    if(deltaHeight < 0){
        freeFallCounter++;
    } else{
        freeFallCounter = 0;
    }

    if(freeFallCounter >= 6){
        Serial.println("Apogeo detectado!!!");
    }

    delay(100);
}