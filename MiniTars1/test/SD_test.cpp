#include "processing/SDLogger.h"

SDLogger sdLogger(4);  // El pin al que esta conectado el CS o Chip Select
State currentState = PROCESS;

void setup() {
    Serial.begin(9600);

    if (!sdLogger.begin()) {
        Serial.println("Error initializing SD logger.");
        while (1);  // Detener si hay error en la inicialización
    }
}

void loop() {
    int altitude = 10;
    int barometer = 10;
    int accelerometer = 10;

    sdLogger.logData(currentState, altitude, barometer, accelerometer);
    delay(1000);
}