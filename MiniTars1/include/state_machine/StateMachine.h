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
#include "configurations/board_pins.h"

class StateMachine {
private:
    RocketState state; // Current state of the rocket
    
    // Subsystems instances
    TARSIMU tarsIMU;
    TARSBMP tarsBMP;
    TARSFLASH tarsFLASH;
    TARSLED tarsLED;
    
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
    Serial.println("State machine begins ;)");

    //* Board Pins initialization
    pins_setup();
    
    //* Subsystems setup
    tarsBMP.setup();
    tarsIMU.setup();
    tarsFLASH.setup();
    tarsLED.setup();

    //* Read Flash saved data
    setState(tarsFLASH.readState());
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
    if (digitalRead(RESET_BUTTON) == HIGH && millis() > 2000) { // If the reset button is pressed, reset control variables
        tarsFLASH.controlVariablesReset();
        //!setState(IDLE); revisar pq aqui no estaría correcto
    }
}

void StateMachine::stateAction() { //* What happens in this state
    switch (state) {
        case IDLE: { // Idle
            tarsLED.changeColor(IDLE);
            Serial.println("Phase:     IDLE");
            break;
        }
        case ARMED: { // Registers initial altitude
            tarsLED.changeColor(ARMED);
            Serial.println("Phase:     ARMED");

            if (tarsFLASH.getInitialAltitude() == NULL) {
                tarsFLASH.setInitialAltitude(tarsBMP.calculateAltitude());
                Serial.println("Initial Altitude" + String(tarsFLASH.getInitialAltitude()));
            }
            break;
        }
        case LAUNCH: { // Registers Data in Flash, but that will be managed as a global task that always happens (in IDLE data is not saved)
            tarsLED.changeColor(LAUNCH);
            Serial.println("Phase:     LAUNCH");

            break;
        }
        case APOGEE: { // Activates Pyro
            tarsLED.changeColor(APOGEE);
            Serial.println("Phase:     APOGEE");

            digitalWrite(PYRO_TRIGGER, HIGH);
            digitalWrite(PYRO_TRIGGER2, HIGH);
            break;
        }
        case RECOVERY: {
            tarsLED.changeColor(RECOVERY);
            Serial.println("Phase:     RECOVERY");
            uint8_t data[5] = {tarsIMU.getAcceleration(), 0, 0};
            tarsFLASH.newRegister(data); //??
            // Parpadear leds
            break;
        }
        case LANDED: {
            Serial.println("Phase:     LANDED");
            tarsLED.changeColor(LANDED); //?
            break;
        }
    }
}

void StateMachine::stateTransition() { //* Reads external signal and changes state
    if(digitalRead(RESET_BUTTON)) { //! lets see if this works
        setState(IDLE);
        break;
    }

    switch (state) {
        case IDLE: { //Change with arming button
            if (digitalRead(ARM_BUTTON) == HIGH)
                setState(ARMED);
            break;
        }
        case ARMED: {
            if (tarsIMU.launchDetection())
                setState(LAUNCH);
            break;
        }
        case LAUNCH: {
            if (tarsBMP.freeFallDetection())
                setState(APOGEE);
            break;
        }
        case APOGEE: {
            if (digitalRead(PYRO_READ) == HIGH && digitalRead(PYRO_READ2) == HIGH)
                setState(RECOVERY);
            break;
        }
        case RECOVERY: {
            //if(info transmitted)
                setState(LANDED);
            break;
        }
        case LANDED: {//?
            break;
        }
    }
}

#endif