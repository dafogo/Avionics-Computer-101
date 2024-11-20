#ifndef StateMachine_h
#define StateMachine_h
#include <Arduino.h>

#include "subsystems\TARSIMU.h"
#include "subsystems\TARSFLASH.h"

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
    StateMachine();

    TARSIMU getTarsIMU() { return tarsIMU; }

    void setup() {
        pinMode(armButton, INPUT);
        tarsIMU.setup();
        tarsFLASH.setup();
    }

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
                if (digitalRead(armButton) == HIGH) { state = ARMED; }
                break;
            }
            case ARMED: {
                if (tarsIMU.launchDetection() == true) { state = LAUNCH; }
                break;
            }
            case LAUNCH: {
                if (tarsIMU.freeFallDetection() == true) { state = APOGEE; }
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
    RocketState state = IDLE;

    // Subsystems
    TARSIMU tarsIMU;
    TARSFLASH tarsFLASH;

    // Arm Button
    int armButton = 5;
};
#endif