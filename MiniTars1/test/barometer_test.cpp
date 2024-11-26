/*
 * @file barometer_test.cpp
 * @date 11/25/2024
 * @author Dana E. Torres Estrada
 * 
 * @brief Test program for the TARSBarometer class.
 */

#include <Arduino.h>
#include "subsystems/TARSBarometer.h"

TARSBarometer barometer;

void setup() {
    // Start the serial communication for debugging
    Serial.begin(115200);
    while (!Serial);

    // Initialize the BMP390 sensor
    if (!barometer.begin()) {
        while (1); // Halt execution if the sensor fails to initialize
    }
}

void loop() {
    // Read and print temperature
    float temperature = barometer.readTemperature();
    if (temperature != -1.0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    } else {
        Serial.println("Failed to read temperature.");
    }

    // Read and print pressure
    float pressure = barometer.readPressure();
    if (pressure != -1.0) {
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    } else {
        Serial.println("Failed to read pressure.");
    }

    // Calculate and print altitude
    float altitude = barometer.calculateAltitude();
    if (altitude != -1.0) {
        Serial.print("Altitude: ");
        Serial.print(altitude);
        Serial.println(" m");
    } else {
        Serial.println("Failed to calculate altitude.");
    }

    // Wait for 2 seconds before the next loop
    delay(2000);
}