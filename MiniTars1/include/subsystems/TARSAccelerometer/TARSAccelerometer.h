#include <Arduino.h>
#include <Adafruit_LSM6DSOX.h>

class IMU_Accelerometer {
    private:
    // For SPI mode, we need a CS pin
    #define LSM_CS 10
    // For software-SPI mode we need SCK/MOSI/MISO pins
    #define LSM_SCK 13
    #define LSM_MISO 12
    #define LSM_MOSI 11

    Adafruit_LSM6DSOX sox;

    /* Get a new normalized sensor event */
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    float ref_ang_a;
    float ref_ang_b;
    float ref_ang_y;

    int dominantAxis;

    public: 

    unsigned int x_displacement = 0;
    unsigned int lastVelocity = 0;

    public:
    IMU_Accelerometer() {}
    void IMU_Setup() {
        if (!sox.begin_I2C()) {
            while (1) {
            delay(10);
            Serial.println("NO JALÓ");
            }
        }
        Serial.println("LSM6DSOX Found!");
        // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
        Serial.print("Accelerometer range set to: ");
        switch (sox.getAccelRange()) {
        case LSM6DS_ACCEL_RANGE_2_G:
            Serial.println("+-2G");
            break;
        case LSM6DS_ACCEL_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case LSM6DS_ACCEL_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case LSM6DS_ACCEL_RANGE_16_G:
            Serial.println("+-16G");
            break;
        }

        // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
        Serial.print("Gyro range set to: ");
        switch (sox.getGyroRange()) {
        case LSM6DS_GYRO_RANGE_125_DPS:
            Serial.println("125 degrees/s");
            break;
        case LSM6DS_GYRO_RANGE_250_DPS:
            Serial.println("250 degrees/s");
            break;
        case LSM6DS_GYRO_RANGE_500_DPS:
            Serial.println("500 degrees/s");
            break;
        case LSM6DS_GYRO_RANGE_1000_DPS:
            Serial.println("1000 degrees/s");
            break;
        case LSM6DS_GYRO_RANGE_2000_DPS:
            Serial.println("2000 degrees/s");
            break;
        case ISM330DHCX_GYRO_RANGE_4000_DPS:
            break; // unsupported range for the DSOX
        }

        // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
        Serial.print("Accelerometer data rate set to: ");
        switch (sox.getAccelDataRate()) {
        case LSM6DS_RATE_SHUTDOWN:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_12_5_HZ:
            Serial.println("12.5 Hz");
            break;
        case LSM6DS_RATE_26_HZ:
            Serial.println("26 Hz");
            break;
        case LSM6DS_RATE_52_HZ:
            Serial.println("52 Hz");
            break;
        case LSM6DS_RATE_104_HZ:
            Serial.println("104 Hz");
            break;
        case LSM6DS_RATE_208_HZ:
            Serial.println("208 Hz");
            break;
        case LSM6DS_RATE_416_HZ:
            Serial.println("416 Hz");
            break;
        case LSM6DS_RATE_833_HZ:
            Serial.println("833 Hz");
            break;
        case LSM6DS_RATE_1_66K_HZ:
            Serial.println("1.66 KHz");
            break;
        case LSM6DS_RATE_3_33K_HZ:
            Serial.println("3.33 KHz");
            break;
        case LSM6DS_RATE_6_66K_HZ:
            Serial.println("6.66 KHz");
            break;
        }

        // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
        Serial.print("Gyro data rate set to: ");
        switch (sox.getGyroDataRate()) {
        case LSM6DS_RATE_SHUTDOWN:
            Serial.println("0 Hz");
            break;
        case LSM6DS_RATE_12_5_HZ:
            Serial.println("12.5 Hz");
            break;
        case LSM6DS_RATE_26_HZ:
            Serial.println("26 Hz");
            break;
        case LSM6DS_RATE_52_HZ:
            Serial.println("52 Hz");
            break;
        case LSM6DS_RATE_104_HZ:
            Serial.println("104 Hz");
            break;
        case LSM6DS_RATE_208_HZ:
            Serial.println("208 Hz");
            break;
        case LSM6DS_RATE_416_HZ:
            Serial.println("416 Hz");
            break;
        case LSM6DS_RATE_833_HZ:
            Serial.println("833 Hz");
            break;
        case LSM6DS_RATE_1_66K_HZ:
            Serial.println("1.66 KHz");
            break;
        case LSM6DS_RATE_3_33K_HZ:
            Serial.println("3.33 KHz");
            break;
        case LSM6DS_RATE_6_66K_HZ:
            Serial.println("6.66 KHz");
            break;
        }

        // Set Reference Cosines
        sox.getEvent(&accel, &gyro, &temp);
        float x_accel = accel.acceleration.x;
        float y_accel = accel.acceleration.y;
        float z_accel = accel.acceleration.z;
        float accelMagnitude = sqrt(pow(x_accel, 2) + pow(y_accel, 2) + pow(z_accel, 2));
        ref_ang_a = acos(x_accel / accelMagnitude);
        ref_ang_b = acos(y_accel / accelMagnitude);
        ref_ang_y = acos(z_accel / accelMagnitude);

        return;
    }

    void getDominantAxis() {
        sox.getEvent(&accel, &gyro, &temp);
        float x_accel = accel.acceleration.x;
        float y_accel = accel.acceleration.y;
        float z_accel = accel.acceleration.z;
        
        // Identify dominant axis (depends on rocket position)
        // Serial.println("Variables Iniciales X = " + String(x_accel) + "     Y = " + String(y_accel) + "     Z = " + String(z_accel));
        dominantAxis = 0;
        if (abs(y_accel) > abs(x_accel)) {
            dominantAxis = 1; 
        }
        if (dominantAxis == 0) { 
            if (abs(z_accel) > abs(x_accel)) {
                dominantAxis = 2;
            }
        } else {
            if (abs(z_accel) > abs(y_accel)) {
                dominantAxis = 2;
            }
        }
    }
    
    float getAccelVector() {
        sox.getEvent(&accel, &gyro, &temp);
        float x_accel = accel.acceleration.x;
        float y_accel = accel.acceleration.y;
        float z_accel = accel.acceleration.z;
        float accelMagnitude = sqrt(pow(x_accel, 2) + pow(y_accel, 2) + pow(z_accel, 2));
        Serial.println("X = " + String(x_accel) + "     Y = " + String(y_accel) + "     Z = " + String(z_accel));
        return accelMagnitude;
    }

    bool flightStatus() {
        getDominantAxis();
        sox.getEvent(&accel, &gyro, &temp);
        float x_accel = abs(accel.acceleration.x);
        float y_accel = abs(accel.acceleration.y);
        float z_accel = abs(accel.acceleration.z);
        bool status = 1;
        // Serial.println(dominantAxis);
        switch (dominantAxis) {
            case 0:
                if (x_accel < 9.2) {
                    Serial.println("X Cayendo");
                    status = 0;
                } else {
                    Serial.println('.');
                }
            break;
            case 1:
                if (y_accel < 9.2) {
                    Serial.println("Y Cayendo");
                    status = 0;
                } else {
                    Serial.println('.');
                }
            break;
            case 2:
                if (z_accel < 9.2) {
                    Serial.println("Z Cayendo");
                    status = 0;
                } else {
                    Serial.println('.');
                }
            break;
        }
        return status;
    }

    void sumDisplacement() {
        unsigned int time0 = millis();
        delay(20);
        sox.getEvent(&accel, &gyro, &temp);

        Serial.println(accel.acceleration.x);

        unsigned int s_timeElapsed = (millis() - time0) * 0.001;    // Time in seconds
        x_displacement = x_displacement + (lastVelocity * (s_timeElapsed)) + ((accel.acceleration.x * pow(s_timeElapsed, 2))/2);
        lastVelocity = lastVelocity + (accel.acceleration.x * s_timeElapsed);

        Serial.print("Time1 = ");
        Serial.println(s_timeElapsed);
    }

    void printValues() {
        // Get a new normalized sensor event
        sox.getEvent(&accel, &gyro, &temp);

        Serial.print("\t\tTemperature ");
        Serial.print(temp.temperature);
        Serial.println(" deg C");

        //Display the results (acceleration is measured in m/s^2)
        Serial.print("\t\tAccel X: ");
        Serial.print(accel.acceleration.x);
        Serial.print(" \tY: ");
        Serial.print(accel.acceleration.y);
        Serial.print(" \tZ: ");
        Serial.print(accel.acceleration.z);
        Serial.println(" m/s^2 ");

        // Display the results (rotation is measured in rad/s)
        Serial.print("\t\tGyro X: ");
        Serial.print(gyro.gyro.x);
        Serial.print(" \tY: ");
        Serial.print(gyro.gyro.y);
        Serial.print(" \tZ: ");
        Serial.print(gyro.gyro.z);
        Serial.println(" radians/s ");
        Serial.println();

        delay(1000);
    }


};