#ifndef StateMachine_h
#define StateMachine_h

/* 
 * Mini Tars Machine State class Definition
 * Diego Fondón & Dana Torres
 */

#include <Arduino.h>

#include "DataStructures.h"
#include "subsystems\TARSIMU.h"
#include "subsystems\TARSFLASH.h"
#include "subsystems\TARSLED.h"
#include "subsystems\TARSBMP.h"
#include "barometer_apogee.h"

class StateMachine {
    public:
    StateMachine() {};

    void setup() {
        pinMode(armButton, INPUT);
        pinMode(resetButton, INPUT);
        Serial.println("Comienza el código: " + String(digitalRead(resetButton)));
        tarsBMP.setup();
        tarsIMU.setup();
        tarsFLASH.setup();
        tarsLED.setup();
    }

    RocketState getState() { state = (RocketState)tarsFLASH.readState(); return state; }

    void setState(uint8_t newState) { tarsFLASH.writeState(newState); state = (RocketState)newState; }

    void resetControlVariables() {
        if (digitalRead(resetButton) == HIGH && millis() > 2000) {
            tarsFLASH.controlVariablesReset();
        }
    }
    void stateAction() {
        switch (state) {
            case IDLE: {
                tarsLED.changeColor(IDLE);
                Serial.print("Altitude: ");
                Serial.println(tarsBMP.calculateAltitude());
                break;
            }
            case ARMED: {
                tarsLED.changeColor(ARMED);
                if (tarsFLASH.getInitialAltitude() == 0) { 
                    tarsFLASH.setInitialAltitude(tarsBMP.calculateAltitude());
                    Serial.println("Initial Altitude" + String(tarsFLASH.getInitialAltitude()));
                }
                break;
            }
            case LAUNCH: {
                tarsLED.changeColor(LAUNCH);
                Serial.println(tarsFLASH.getInitialAltitude());
                uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
                tarsFLASH.newRegister(data);
                break;
            }
            case APOGEE: {
                tarsLED.changeColor(APOGEE);
                uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
                tarsFLASH.newRegister(data);
                break;
            }
            case RECOVERY: {
                tarsLED.changeColor(RECOVERY);
                uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
                tarsFLASH.newRegister(data);
                break;
            }
            case LANDED: {
                tarsLED.changeColor(LANDED);
                break;
            }
        }
    }

    void stateTransition() {
        switch (state) {
            case IDLE: {
                if (digitalRead(armButton) == HIGH) { setState(ARMED); }
                break;
            }
            case ARMED: {
                if (tarsBMP.calculateAltitude() > tarsFLASH.getInitialAltitude() + 0.5) { setState(LAUNCH); }
                break;
            }
            case LAUNCH: {
                // if (tarsIMU.freeFallDetection() == true) { setState(APOGEE); }
                // if (freeFallDetection(tarsBMP)) { setState(APOGEE); }
                if (freeFallDetection(tarsBMP)) { Serial.println("APOGEE"); }
                break;
            }
            case APOGEE: {
                break;
            }
            case RECOVERY: {
                break;
            }
            case LANDED: {
                break;
            }
        }
    }
    private:
    RocketState state;

    // Subsystems
    TARSIMU tarsIMU;
    TARSFLASH tarsFLASH;
    TARSLED tarsLED;
    TARSBMP tarsBMP;

    // Arm Button
    int armButton = 18;

    // Reset Button
    int resetButton = 5;
};
#endif