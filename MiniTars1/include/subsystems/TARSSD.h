/*
 * @file TARSSD.h
 * @date 11/12/2024
 * @author Dana E. Torres Estrada
 * 
 * @brief Header file for managing SD card logging.
 * Includes methods for initializing the SD card, creating a CSV file, and logging measurement data (time, state, altitude, pitch, roll, yaw)
 */
#ifndef TARSSD_H
#define TARSSD_H

#include <SD.h>

#include "state_machine/RocketState.h"
#include "TARSFLASH.h"

class SDLogger {
private:
    const int chipSelect;
    String filename;
    File dataFile;

    // Private function to convert state to string
    String stateToString(RocketState state);

    // Private function to convert ms to HH:MM:SS:ms format
    String millisToTime(unsigned long milliseconds);

public:
    SDLogger(int csPin) : chipSelect(csPin), filename("") {} // Constructor

    // Initialize SD card and create file with unique name
    bool setup();

    // Function to log data to the SD card
    void logData(RocketState state, float altitude, float pitch, float roll, float yaw);
};

// ----------------------------------------------------------
// Method definitions
// ----------------------------------------------------------

bool SDLogger::setup() {
    pinMode(chipSelect, OUTPUT);

    if (!SD.begin(chipSelect)) {
        Serial.println("SD initialization failed!");
        return false;
    }

    int fileNum = 1;
    //if (tarsFLASH.getCSVFilename() == 0) { do...}
    do { //!cambiar a que lo cheque con la flash, en caso de que se reinicie la computadora durante el vuelo
        filename = "data" + String(fileNum) + ".csv";
        fileNum++;
    } while (SD.exists(filename));

    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        dataFile.println("Time(H:MM:SS:ms),Phase,Altitude(m),Pitch(degrees), Roll(degrees), Yaw(degrees)");
        dataFile.close();
        Serial.println("File created: " + filename);
        return true;
    } else {
        Serial.println("Error creating file.");
        return false;
    }
}

void SDLogger::logData(RocketState state, float altitude, float pitch, float roll, float yaw) {
    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        String currentTime = millisToTime(millis());
        String data = currentTime + "," + stateToString(state) + "," +
                        String(altitude) + "," + String(pitch) + "," + String(roll) + "," + String(yaw);
        dataFile.println(data);
        dataFile.close();
        Serial.println("Data written: " + data);
    } else {
        Serial.println("Error opening file for writing data.");
    }
}

// Private Methods

String SDLogger::stateToString(RocketState state) {
    switch (state) {
        case IDLE: return "IDLE";
        case ARMED: return "ARMED";
        case LAUNCH: return "LAUNCH";
        case APOGEE: return "APOGEE";
        case RECOVERY: return "RECOVERY";
        case LANDED: return "LANDED";
        default: return "UNKNOWN";
    }
}

String SDLogger::millisToTime(unsigned long milliseconds) {
    unsigned long hours = milliseconds / 3600000;
    milliseconds %= 3600000;
    unsigned long minutes = milliseconds / 60000;
    milliseconds %= 60000;
    unsigned long seconds = milliseconds / 1000;
    milliseconds %= 1000;

    String timeString = String(hours) + ":" +
                        (minutes < 10 ? "0" : "") + String(minutes) + ":" +
                        (seconds < 10 ? "0" : "") + String(seconds) + ":" +
                        (milliseconds < 100 ? (milliseconds < 10 ? "00" : "0") : "") + String(milliseconds);
    
    return timeString;
}

#endif