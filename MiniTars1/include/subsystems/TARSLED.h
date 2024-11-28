#ifndef TARS_LED_H
#define TARS_LED_H

/* 
 * Mini Tars IMU class Definition
 * Diego Fondón & Dana Torres
 */

#include <Adafruit_NeoPixel.h>

#include "DataStructures.h"

class TARSLED {
    public:
    TARSLED() {}

    void setup() { tarsStrip.begin(); }

    void changeColor(RocketState rocketState);

    private:
    const int stripLength = 8;
    const int stripPin = 4;
    Adafruit_NeoPixel tarsStrip = Adafruit_NeoPixel(stripLength, stripPin);
};

void TARSLED::changeColor(RocketState rocketState) {
    switch (rocketState) {
        case IDLE: {
            tarsStrip.fill(tarsStrip.Color(255, 0, 0), 0, 0);
            tarsStrip.show();
            break;
        }
        case ARMED: {
            tarsStrip.fill(tarsStrip.Color(0, 255, 0), 0, 0);
            tarsStrip.show();
            break;
        }
        case LAUNCH: {
            tarsStrip.fill(tarsStrip.Color(255, 116, 0), 0, 0);
            tarsStrip.show();
            break;
        }
        case APOGEE: {
            tarsStrip.fill(tarsStrip.Color(0, 0, 255), 0, 0);
            tarsStrip.show();
            break;
        }
        case RECOVERY: {
            tarsStrip.fill(tarsStrip.Color(116, 0, 255), 0, 0);
            tarsStrip.show();
            break;
        }
        case LANDED: {
            tarsStrip.fill(tarsStrip.Color(255, 193, 0), 0, 0);
            tarsStrip.show();
            break;
        }
    }
}

#endif