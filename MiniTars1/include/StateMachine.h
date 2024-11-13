#ifndef StateMachine_h
#define StateMachine_h

enum States {
    IDLE = 1,
    LAUNCH = 2,
    APOGEE = 3, 
    RECOVERY = 4, 
    LANDED = 5
};

class StateMachine {
    void stateCallback() {
        switch (state) {
            case IDLE: {
                // Store data ON BUFFER method
                break;
            }
            case LAUNCH: {
                // Store data method
            }
            case APOGEE: {
                // Deploy parachute
            }
            case RECOVERY: {
                // Store data method
            }
            case LANDED: {
                // Data transfer to SD Card
            }
        }
    }
    private:
    States state = IDLE;
};
#endif