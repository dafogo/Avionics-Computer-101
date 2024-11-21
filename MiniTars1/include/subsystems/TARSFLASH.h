#ifndef TARS_FLASH_H
#define TARS_FLASH_H
/* 
 * Mini Tars EEPROM class Definition
 * Diego Fondón & Dana Torres
 */
#include <Adafruit_EEPROM_I2C.h>

#define EEPROM_ADDR 0x50  // The default address!

class TARSFLASH {
    public:

    TARSFLASH() {};
    
    void setup() {
        if (i2ceeprom.begin(0x50)) { Serial.println("Found I2C EEPROM"); } 
        else {
            Serial.println("I2C EEPROM not identified ... check your connections?\r\n");
            while (1) delay(10);
        }
    }

    // Grabado de variables de control en eeprom
    void controlVariablesReset() {
        i2ceeprom.write(addrState, 1);
        i2ceeprom.write(addrRegisterStart, 4);
        i2ceeprom.write(addrRegisterCounter, 0);
    }

    uint8_t readState() {
        return i2ceeprom.read(addrState);
    }

    void writeState(uint8_t state) {
        i2ceeprom.write(addrState, state);
    }

    void newRegister(uint8_t data[]) {
        uint32_t actualRegisterCounter = i2ceeprom.read(addrRegisterCounter);
        uint32_t inicioEscritura = i2ceeprom.read(addrRegisterStart) + registerSizeInBytes * actualRegisterCounter;

        for(uint8_t i = 0; i < registerSizeInBytes; i++) { i2ceeprom.write(inicioEscritura + i, data[i]); }

        i2ceeprom.write(addrRegisterCounter, ++actualRegisterCounter);
    }

    private:
    const uint8_t addrState = 0;
    const uint8_t addrRegisterStart = 1;
    const uint8_t addrRegisterCounter = 2;
    const uint8_t registerSizeInBytes = 5;
    Adafruit_EEPROM_I2C i2ceeprom;
}
;
#endif