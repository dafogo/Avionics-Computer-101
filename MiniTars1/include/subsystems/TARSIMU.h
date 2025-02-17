/* 
 * Mini Tars IMU class Definition
 * Diego Fondón & Dana Torres
 */

#ifndef TARS_IMU_H
#define TARS_IMU_H

#include <Adafruit_LSM6DSOX.h>

class TARSIMU {
    private:
    Adafruit_LSM6DSOX sox;

    /* Get a new normalized sensor event */
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    /* Dominant Axis */
    int dominantAxis;

    int freeFallCounter = 0;

    int statusLED = 2;

    public:

    TARSIMU() {};
    
    void setup();   // ci

    uint8_t getAcceleration();  // ci

    int getStatusLED() { return statusLED; }    // no está en el otro

    void getDominantAxis(); // Dana lo hizo mejor

    bool flightStatus();    // ya no está

    bool freeFallDetection();

    bool launchDetection();
};

void TARSIMU::setup() {
    if (!sox.begin_I2C()) {
        while (1) {
        delay(10);
        Serial.println("NO JALÓ");
        }
    }
    Serial.println("LSM6DSOX Found!");
    pinMode(statusLED, OUTPUT);
}

uint8_t TARSIMU::getAcceleration() {
    sox.getEvent(&accel, &gyro, &temp);
    return accel.acceleration.x, accel.acceleration.y, accel.acceleration.z;
}

void TARSIMU::getDominantAxis() {
    sox.getEvent(&accel, &gyro, &temp);
    float x_accel = accel.acceleration.x;
    float y_accel = accel.acceleration.y;
    float z_accel = accel.acceleration.z;
    
    // Identify dominant axis (depends on rocket position)
    // Serial.println("Variables Iniciales X = " + String(x_accel) + "     Y = " + String(y_accel) + "     Z = " + String(z_accel));
    dominantAxis = 0;
    if (abs(y_accel) > abs(x_accel)) {
        dominantAxis = 1; 
    }
    if (dominantAxis == 0) { 
        if (abs(z_accel) > abs(x_accel)) {
            dominantAxis = 2;
        }
    } else {
        if (abs(z_accel) > abs(y_accel)) {
            dominantAxis = 2;
        }
    }
}

bool TARSIMU::flightStatus() {
    getDominantAxis();
    sox.getEvent(&accel, &gyro, &temp);
    float x_accel = abs(accel.acceleration.x);
    float y_accel = abs(accel.acceleration.y);
    float z_accel = abs(accel.acceleration.z);
    bool status = 1;
    // Serial.println(dominantAxis);
    switch (dominantAxis) {
        case 0:
            if (x_accel < 9.2) {
                Serial.println("X Cayendo");
                status = 0;
            } else {
                Serial.println('.');
            }
        break;
        case 1:
            if (y_accel < 9.2) {
                Serial.println("Y Cayendo");
                status = 0;
            } else {
                Serial.println('.');
            }
        break;
        case 2:
            if (z_accel < 9.2) {
                Serial.println("Z Cayendo");
                status = 0;
            } else {
                Serial.println('.');
            }
        break;
    }
    return status;
}

bool TARSIMU::freeFallDetection() {
    // Usa variable para asegurar caída
    if (flightStatus() == 0) { freeFallCounter++; }
    else { freeFallCounter = 0; }
    
    if (freeFallCounter == 6) {
        Serial.println("Abriendo Paracaídas");
        digitalWrite(statusLED, HIGH);
        return true;
    }
    return false;
}

bool TARSIMU::launchDetection() {
    if (flightStatus() == 0) {return true;}
    else { return false; }
}
#endif