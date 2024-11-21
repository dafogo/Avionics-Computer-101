#ifndef StateMachine_h
#define StateMachine_h
#include <Arduino.h>

#include "subsystems\TARSIMU.h"
#include "subsystems\TARSFLASH.h"
#include "subsystems\TARSLED.h"

enum RocketState {
    IDLE = 1,
    ARMED = 2,
    LAUNCH = 3,
    APOGEE = 4, 
    RECOVERY = 5, 
    LANDED = 6
};

class StateMachine {
    public:
    StateMachine() {};

    void setup() {
        pinMode(armButton, INPUT);
        tarsIMU.setup();
        tarsFLASH.setup();
        tarsLED.setup();
    }

    RocketState getState() { state = (RocketState)tarsFLASH.readState(); return state; }

    void setState(uint8_t newState) { tarsFLASH.writeState(newState); state = (RocketState)newState; }

    void stateAction() {
        switch (state) {
            case IDLE: {
                break;
            }
            case ARMED: {
                break;
            }
            case LAUNCH: {
                // Store data method
                break;
            }
            case APOGEE: {
                // Deploy parachute
                break;
            }
            case RECOVERY: {
                // Store data method
                break;
            }
            case LANDED: {
                // Data transfer to SD Card
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
};
#endif