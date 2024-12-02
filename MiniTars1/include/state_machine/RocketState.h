#ifndef ROCKETSTATE_H
#define ROCKETSTATE_H

enum RocketState {
    IDLE = 1,
    ARMED = 2,
    LAUNCH = 3,
    APOGEE = 4, 
    RECOVERY = 5, 
    LANDED = 6
};

#endif