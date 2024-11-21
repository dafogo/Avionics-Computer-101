#ifndef TARS_LED_H
#define TARS_LED_H

/* 
 * Mini Tars IMU class Definition
 * Diego Fondón & Dana Torres
 */

#include <Adafruit_NeoPixel.h>

class TARSLED {
    public:
    TARSLED() {};

    void setup() { tarsStrip.begin(); }

    private:
    const int stripLength = 8;
    const int stripPin = 4;
    Adafruit_NeoPixel tarsStrip = Adafruit_NeoPixel(stripLength, stripPin);
};
#endif