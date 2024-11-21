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

class StateMachine {
    public:
    StateMachine() {};

    void setup() {
        pinMode(armButton, INPUT);
        pinMode(resetButton, INPUT);
        tarsIMU.setup();
        tarsFLASH.setup();
        tarsLED.setup();
    }

    RocketState getState() { state = (RocketState)tarsFLASH.readState(); return state; }

    void setState(uint8_t newState) { tarsFLASH.writeState(newState); state = (RocketState)newState; }

    void resetControlVariables() {
        if (digitalRead(resetButton) == HIGH) {
            tarsFLASH.controlVariablesReset();
        }
    }
    void stateAction() {
        switch (state) {
            case IDLE: {
                tarsLED.changeColor(IDLE);
                break;
            }
            case ARMED: {
                tarsLED.changeColor(ARMED);
                break;
            }
            case LAUNCH: {
                tarsLED.changeColor(LAUNCH);
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
                if (tarsIMU.launchDetection() == true) { setState(LAUNCH); }
                break;
            }
            case LAUNCH: {
                if (tarsIMU.freeFallDetection() == true) { setState(APOGEE); }
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

    // Arm Button
    int armButton = 5;

    // Reset Button
    int resetButton = 4;
};
#endif