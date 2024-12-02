/* 
 * Mini Tars Machine State class Definition
 * Diego Fondón & Dana Torres
 */

#ifndef StateMachine_h
#define StateMachine_h

#include <Arduino.h>

#include "RocketState.h"
#include "subsystems/TARSIMU.h" //!checar esto
#include "subsystems/TARSBMP.h"
#include "subsystems/TARSFLASH.h"
#include "subsystems/TARSLED.h"
#include "processing/BMPprocess.h"

class StateMachine {
private:
    RocketState state; // Current state of the rocket
    
    // Subsystems instances
    TARSIMU tarsIMU;
    TARSBMP tarsBMP;
    TARSFLASH tarsFLASH;
    TARSLED tarsLED;

    // Button pins
    const int armButton = 18; //!cheacar si es conveniente que esto este en el constructor
    const int resetButton = 5;
    
public:
    StateMachine(); // Constructor

    // Setup method for initialization
    void setup();
    
    // Returns the current state
    RocketState getState();
    // Sets a new state
    void setState(uint8_t newState);

    // Resets control variables
    void resetControlVariables();
    // Performs the action based on the current state
    void stateAction();
    // Checks for state transitions and triggers them
    void stateTransition();
};

// ----------------------------------------------------------
// Method definitions
// ----------------------------------------------------------

void StateMachine::setup() {
    pinMode(armButton, INPUT); // Initialization Arming Button
    
    pinMode(resetButton, INPUT); // Initialization Reset Button
    Serial.println("Comienza el código: " + String(digitalRead(resetButton)));
    
    // Subsystems setup
    tarsBMP.setup();
    tarsIMU.setup();
    tarsFLASH.setup();
    tarsLED.setup();
}

RocketState StateMachine::getState() {
    state = (RocketState)tarsFLASH.readState(); // Get the current state from flash memory
    return state;
}

void StateMachine::setState(uint8_t newState) {
    tarsFLASH.writeState(newState); // Set a new state and store it in flash memory
    state = (RocketState)newState;
}

void StateMachine::resetControlVariables() {
    if (digitalRead(resetButton) == HIGH && millis() > 2000) { // If the reset button is pressed, reset control variables
        tarsFLASH.controlVariablesReset();
    }
}

void StateMachine::stateAction() {
    switch (state) {
        case IDLE: { //idle
            tarsLED.changeColor(IDLE);
            Serial.print("Altitude: ");
            Serial.println(tarsBMP.calculateAltitude());
            break;
        }
        case ARMED: { //Registers initial altitude
            tarsLED.changeColor(ARMED);
            if (tarsFLASH.getInitialAltitude() == 0) {
                tarsFLASH.setInitialAltitude(tarsBMP.calculateAltitude());
                Serial.println("Initial Altitude" + String(tarsFLASH.getInitialAltitude()));
            }
            break;
        }
        case LAUNCH: {
            tarsLED.changeColor(LAUNCH);
            uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
            tarsFLASH.newRegister(data); //?
            break;
        }
        case APOGEE: {
            tarsLED.changeColor(APOGEE);
            uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
            tarsFLASH.newRegister(data); //?
            //accionar pyro
            break;
        }
        case RECOVERY: {
            tarsLED.changeColor(RECOVERY);
            uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
            tarsFLASH.newRegister(data); //??
            // Parpadear leds
            break;
        }
        case LANDED: {
            tarsLED.changeColor(LANDED); //?
            break;
        }
    }
}

void StateMachine::stateTransition() {
    switch (state) {
        case IDLE: { //Change with arming button
            if (digitalRead(armButton) == HIGH)
                setState(ARMED);
            break;
        }
        case ARMED: { //!cambiar a deteccion con imu
            /*if (tarsBMP.launchDetection(tarsFLASH.getInitialAltitude())) //!checar esto
                setState(LAUNCH);*/
            
            if (tarsIMU.launchDetection())
                setState(LAUNCH);
            break;
        }
        case LAUNCH: { //!Change with BMP apogee detection
            if (tarsBMP.freeFallDetection())
                setState(APOGEE);
            /*if (tarsIMU.freeFallDetection())
                setState(APOGEE);*/
            break;
        }
        case APOGEE: { //?
            break;
        }
        case RECOVERY: { //?
            break;
        }
        case LANDED: {//?
            break;
        }
    }
}

#endif