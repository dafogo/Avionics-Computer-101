/*
 * @file TARSBarometer.h
 * @date 11/25/2024
 * @author Dana E. Torres Estrada
 * 
 * @brief Header file for managing the BMP390 barometer sensor, connected via I2C. 
 * Includes methods for initializing the sensor, reading pressure data and calculating altitude; it uses BMP3XX Adafruit library.
 */

#ifndef TARSBAROMETER_H
#define TARSBAROMETER_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

class TARSBarometer {
  private:
    Adafruit_BMP3XX bmp;
    const float SEALEVELPRESSURE_HPA = 1013.25; // Sea level pressure in hPa

  public:
    TARSBarometer() {} // Constructor

    // Initialize the BMP390 sensor
    bool begin();

    // Read pressure from the BMP390 sensor
    float readPressure();

    // Read temperature from sensor
    float readTemperature();

    // Calculate the altitude using the pressure reading
    float calculateAltitude(); //using BMP3XX library, input pressure is not needed
};

// ----------------------------------------------------------
// Method definitions
// ----------------------------------------------------------

bool TARSBarometer::begin() {
    // Start I2C communication
    if (!bmp.begin_I2C()) {
        Serial.println("BMP390 initialization failed!");
        return false;
    }

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X); //? <- Esto es configurable
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    
    Serial.println("BMP390 sensor initialized successfully.");
    return true;
}

float TARSBarometer::readPressure() {
    // Check performance
    if (!bmp.performReading()) {
        Serial.println("Failed to perform pressure reading :(");
        return -1.0; // Return error value
    }

    return bmp.pressure / 100.0; // Convert to hPa
}

float TARSBarometer::readTemperature(){
    // Check performance
    if (!bmp.performReading()) {
        Serial.println("Failed to perform temperature reading :(");
        return -1.0; // Return error value
    }

    return bmp.temperature;
}

float TARSBarometer::calculateAltitude() {
    // Check performance
    if (!bmp.performReading()) {
        Serial.println("Failed to perform altitude reading :(");
        return -1.0; // Return error value
    }

    return bmp.readAltitude(SEALEVELPRESSURE_HPA);
}

#endif