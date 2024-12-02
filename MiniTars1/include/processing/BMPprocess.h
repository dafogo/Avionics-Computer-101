#include <Arduino.h>
#include "subsystems/TARSBMP.h"

float lastHeight = 0.0; // h1
float currentHeight = 0.0; // h2
float deltaHeight = 0.0;

int counter = 0;

void begin(){
    lastHeight = 0.0; // h1
    currentHeight = 0.0; // h2
    deltaHeight = 0.0;
}

bool freeFallDetection(TARSBMP &barometer){
    currentHeight = barometer.calculateAltitude();
    deltaHeight = currentHeight - lastHeight;

    lastHeight = currentHeight;

    if(deltaHeight < 0){
        counter++;
    } else{
        counter = 0;
    }

    if(counter >= 6){
        Serial.println("Barometro — Apogeo detectado!!!");
        return true;
    }
    return false;
}