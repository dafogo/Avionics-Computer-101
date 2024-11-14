/*
 * @file SDLogger.h
 * @date 11/12/2024
 * @author Dana E. Torres Estrada
 * 
 * @brief Header file for managing SD card logging.
 * Includes methods for initializing the SD card, creating a CSV file, and logging measurement data (time, state, altitude, barometer, and accelerometer)
 */
#ifndef SDLOGGER_H
#define SDLOGGER_H

#include <SD.h>

enum State { //! esto en realidad será sustituido al hacer include de la state machine del proyecto
    START,
    PROCESS,
    FINISH
};

class SDLogger {
private:
    const int chipSelect;
    String filename;
    File dataFile;

    // Private function to convert state to string
    String stateToString(State state);//! falta actualizar con la state machine del proyecto

    // Private function to convert ms to HH:MM:SS:ms format
    String millisToTime(unsigned long milliseconds);

public:
    SDLogger(int csPin) : chipSelect(csPin), filename("") {} // Constructor

    // Initialize SD card and create file with unique name
    bool begin();

    // Function to log data to the SD card
    void logData(State state, int altitude, int barometer, int accelerometer);
};

// ----------------------------------------------------------
// Method definitions
// ----------------------------------------------------------

bool SDLogger::begin() {
    pinMode(chipSelect, OUTPUT);

    if (!SD.begin(chipSelect)) {
        Serial.println("SD initialization failed!");
        return false;
    }

    int fileNum = 1;
    do {
        filename = "data" + String(fileNum) + ".csv";
        fileNum++;
    } while (SD.exists(filename));

    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        dataFile.println("Time(H:MM:SS:ms),Phase,Altitude(m),Barometer(m),Accelerometer(m)");
        dataFile.close();
        Serial.println("File created: " + filename);
        return true;
    } else {
        Serial.println("Error creating file.");
        return false;
    }
}

void SDLogger::logData(State state, int altitude, int barometer, int accelerometer) {
    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        String currentTime = millisToTime(millis());
        String data = currentTime + "," + stateToString(state) + "," +
                        String(altitude) + "," + String(barometer) + "," + String(accelerometer);
        dataFile.println(data);
        dataFile.close();
        Serial.println("Data written: " + data);
    } else {
        Serial.println("Error opening file for writing data.");
    }
}

// Private Methods

String SDLogger::stateToString(State state) {
    switch (state) {
        case START: return "START";
        case PROCESS: return "PROCESS";
        case FINISH: return "FINISH";
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