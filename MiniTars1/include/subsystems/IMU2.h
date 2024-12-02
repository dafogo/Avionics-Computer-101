/*
 * @file TARSIMU.h
 * @date 11/25/2024
 * @author Diego Fondón & Dana Torres
 * 
 * @brief Header file for managing the Accelerometers and Gyroscopes of Adafruit LSM6DSOX + LIS3MDL 9 Dof, connected via I2C. 
 * Includes methods for initializing the sensor, reading acceleration and inclination data, freeFallDetection and launchDetection; it uses LSM6DSOX Adafruit library //TODO:...
 */

#ifndef TARS_IMU_H
#define TARS_IMU_H

#include <Adafruit_LSM6DSOX.h>

// Vector3 structure for 3D measurements
struct Vector3 {
    float x, y, z;
    Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Get the absolute value of the dominant axis
    float dominantAccel(int dominantAxis) const {
        switch (dominantAxis) {
            case 0: return abs(x); // X-axis
            case 1: return abs(y); // Y-axis
            case 2: return abs(z); // Z-axis
            default: 
                Serial.println("Invalid dominant axis!");
                return 0.0f;
        }
    }
};


class TARSIMU {
private:
    Adafruit_LSM6DSOX sox;

    int dominantAxis;
    int freeFallCounter = 0;

    const float FREE_FALL_THRESHOLD = 9.2; // in m/s^2
    const int FREE_FALL_COUNT_LIMIT = 6;   // Number of consecutive events to detect free fall
    const float LAUNCH_THRESHOLD = 9.81*2.0;   // in m/s^2 //?<-checar

public:
    TARSIMU() {} // Constructor

    // Initialize the sensor and check connectivity
    bool setup();
    void setDominantAxis();
    int getDominantAxis(); //? lo pensaba para que no se este reescribiendo todo el tiempo, pero talvez no sea necesario

    //* Data Acquisition
    // Returns Accelerations in X, Y and Z axes
    Vector3 getAcceleration();
    // Returns Orientations in X, Y and Z axes
    Vector3 getInclination(); // X-axis = Pitch, Y-axis = Roll, Z-axis = Yaw //TODO:esta mal mal mal falta probar

    //* Motion Detection
    // Launch detection
    bool launchDetection();
    // Apogee detection
    bool freeFallDetection();
};

// ----------------------------------------------------------
// Method definitions
// ----------------------------------------------------------

//* Data Acquisition
bool TARSIMU::setup() {
    // Start I2C communication
    if (!sox.begin_I2C()) {
        Serial.println("IMU initialization failed ;(");
        return false;
    }
    Serial.println("LSM6DSOX Found!");
    return true;
}

void TARSIMU::setDominantAxis() {
    Vector3 accel = getAcceleration();
    float maxAccel = max(max(abs(accel.x), abs(accel.y)), abs(accel.z));
    
    if (maxAccel == abs(accel.x))
        dominantAxis = 0; // X-axis
    else if (maxAccel == abs(accel.y))
        dominantAxis = 1; // Y-axis
    else
        dominantAxis = 2; // Z-axis
}

int TARSIMU::getDominantAxis(){
    return dominantAxis;
}

Vector3 TARSIMU::getAcceleration() {
    sensors_event_t accel, gyro, temp;
    sox.getEvent(&accel, &gyro, &temp);
    return {accel.acceleration.x, accel.acceleration.y, accel.acceleration.z};
}
	
Vector3 TARSIMU::getInclination() { //?<- codigo de internet, falta probar.e integrar el magnetometro para que yaw jale bien ;)
    static Vector3 inclination(0.0f, 0.0f, 0.0f);
    static unsigned long lastTime = millis();

    // Get current sensor data
    sensors_event_t accelEvent, gyroEvent, tempEvent;
    sox.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    // Accelerometer data
    float accelX = accelEvent.acceleration.x;
    float accelY = accelEvent.acceleration.y;
    float accelZ = accelEvent.acceleration.z;

    // Gyroscope data
    float gyroX = gyroEvent.gyro.x * 180.0 / M_PI; // Convert rad/s to deg/s
    float gyroY = gyroEvent.gyro.y * 180.0 / M_PI;
    float gyroZ = gyroEvent.gyro.z * 180.0 / M_PI;

    // Calculate delta time (dt)
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0f; // Convert to seconds
    lastTime = currentTime;

    // Calculate pitch and roll from accelerometer
    float accelPitch = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180.0 / M_PI;
    float accelRoll = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / M_PI;

    // Integrate gyroscope data
    float gyroPitch = inclination.x + gyroX * dt;
    float gyroRoll = inclination.y + gyroY * dt;
    float gyroYaw = inclination.z + gyroZ * dt; // Gyroscope provides yaw directly

    const float alpha = 0.98f;
    inclination.x = alpha * gyroPitch + (1 - alpha) * accelPitch;
    inclination.y = alpha * gyroRoll + (1 - alpha) * accelRoll;
    inclination.z = gyroYaw;

    Serial.print("Inclination - Pitch: ");
    Serial.print(inclination.x);
    Serial.print(" Roll: ");
    Serial.print(inclination.y);
    Serial.print(" Yaw: ");
    Serial.println(inclination.z);

    return inclination;
}

//* Motion Detection

bool TARSIMU::launchDetection() {
    Vector3 accel = getAcceleration();

    float dominantAccel = accel.dominantAccel(getDominantAxis());

    return dominantAccel > LAUNCH_THRESHOLD;
}

bool TARSIMU::freeFallDetection() {
    Vector3 accel = getAcceleration();

    float dominantAccel = accel.dominantAccel(getDominantAxis());
    
    if (dominantAccel < FREE_FALL_THRESHOLD)
        freeFallCounter++;
    else
        freeFallCounter = 0;

    if (freeFallCounter >= FREE_FALL_COUNT_LIMIT) {
        Serial.println("IMU — Apogeo detectado!!!");
        return true;
    }

    return false;
}

#endif