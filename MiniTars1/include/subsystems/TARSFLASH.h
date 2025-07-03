#ifndef TARS_FLASH_H
#define TARS_FLASH_H
/* 
 * Mini Tars EEPROM class Definition
 * Diego Fondón & Dana Torres
 */
#include <Adafruit_EEPROM_I2C.h>

#define EEPROM_ADDR 0x50  // The default address!

class TARSFLASH {
    private:
    // Control Variables Addresses
    const uint8_t addrState = 0;
    const uint8_t addrRegisterStart = 1;
    const uint8_t addrRegisteredBytes = 2;
    const uint8_t addrInitialAltitude = 5;
    // Object Declaration
    Adafruit_EEPROM_I2C i2ceeprom;

    public:
    TARSFLASH() {};
    
    // Initialize the EEPROM
    void setup();

    void setInitialAltitude(float altitude);

    float getInitialAltitude();

    void eepromReset();

    // Grabado de variables de control en eeprom
    void controlVariablesReset();

    uint8_t readState();

    void writeState(uint8_t state);

    void newRegister(unsigned long timestamp, RocketState state, float altitude, Vector3 inclinationVector, Vector3 accelerationVector);

};

void TARSFLASH::setup() {
    if (i2ceeprom.begin(0x50)) { Serial.println("Found I2C EEPROM"); } 
    else {
        Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
        while (1) delay(10);
    }
}

void TARSFLASH::eepromReset() {
    
}

void TARSFLASH::setInitialAltitude(float altitude) {
    Serial.println(i2ceeprom.writeObject(addrInitialAltitude, altitude));
}

float TARSFLASH::getInitialAltitude() {
    float altitude;
    i2ceeprom.readObject(addrInitialAltitude, altitude);
    return altitude;
}

void TARSFLASH::controlVariablesReset() {
    i2ceeprom.write(addrState, 1);
    i2ceeprom.writeObject(addrInitialAltitude, (float)0);
    i2ceeprom.write(addrRegisterStart, 12);
    i2ceeprom.writeObject(addrRegisteredBytes, (uint16_t)0);
}

uint8_t TARSFLASH::readState() {
    return i2ceeprom.read(addrState);
}

void TARSFLASH::writeState(uint8_t state) {
    i2ceeprom.write(addrState, state);
}

void TARSFLASH::newRegister(unsigned long timestamp, RocketState state, float altitude, Vector3 inclinationVector, Vector3 accelerationVector) {

    uint16_t actualRegisteredBytes;
    i2ceeprom.readObject(addrRegisteredBytes, actualRegisteredBytes); // !La variable de control "registeredBytes" debe ser un uint16_t

    if (actualRegisteredBytes < 63700) {    // Until reaching max. memory capacity

        uint16_t RegisterStart= i2ceeprom.read(addrRegisterStart);
        uint16_t inicioEscritura = RegisterStart + actualRegisteredBytes;   // Suma: byte donde comienzan registros + bytes ocupados por registros  

        // Escribir mediciones:
        inicioEscritura = inicioEscritura + i2ceeprom.writeObject(inicioEscritura, timestamp);
        inicioEscritura = inicioEscritura + i2ceeprom.writeObject(inicioEscritura, state);
        inicioEscritura = inicioEscritura + i2ceeprom.writeObject(inicioEscritura, altitude);
        inicioEscritura = inicioEscritura + i2ceeprom.writeObject(inicioEscritura, inclinationVector);
        inicioEscritura = inicioEscritura + i2ceeprom.writeObject(inicioEscritura, accelerationVector);

        // Actualizar cantidad de bytes escritos
        i2ceeprom.writeObject(addrRegisteredBytes, inicioEscritura - RegisterStart);

    } else {    // transfer measures to SD Card when eeprom is full

    }
}
    
#endif