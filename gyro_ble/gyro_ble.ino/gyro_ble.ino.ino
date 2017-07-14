/*
 Copyright (c) 2015 Intel Corporation. All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

 Modified by Jose Nunez @ intel corporation on March 24 2017

 https://www.arduino.cc/en/Tutorial/Genuino101CurieBLEHeartRateMonitor
 
 */

/*
 This sketch example demonstrates how the BMI160 on the
 Intel(R) Curie(TM) module can be used to read accelerometer data
 */

#include "CurieIMU.h"
#include <CurieBLE.h>

/****************** BLE HEARTRATE GLOBALS **************************/
BLEPeripheral blePeripheral; // BLE Peripheral Device (the board you're programming)
BLEService heartRateService("180D"); // BLE Heart Rate Service

// BLE Heart Rate Measurement Characteristic"
BLECharacteristic gyroRateChar("2A37", // standard 16-bit characteristic UUID
        BLERead | BLENotify, 2); // remote clients will be able to get notifications if this characteristic changes
// the characteristic is 2 bytes long as the first field needs to be "Flags" as per BLE specifications
// https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml

int oldgyroRate = 0; // last heart rate reading from analog input
long previousMillis = 0; // last time the heart rate was checked, in ms

/****************** END OF BLE HEARTRATE GLOBALS **************************/

void setup() {
    delay(3000);
    Serial.begin(9600); // initialize Serial communication
    pinMode(13, OUTPUT); // initialize the LED on pin 13 to indicate when a central is connected
    while (!Serial); // wait for the serial port to open
    Serial.println("GYRO BLE SETUP...");
    // initialize device
    Serial.println("Initializing IMU device...");
    CurieIMU.begin();

    // Set the accelerometer range to 2G
    CurieIMU.setAccelerometerRange(2);

    /****************** BLE HEARTRATE SETUP **************************/
    /* Set a local name for the BLE device
    This name will appear in advertising packets
    and can be used by remote devices to identify this BLE device
    The name can be changed but maybe be truncated based on space left in advertisement packet */
    blePeripheral.setLocalName("HeartRateSketch");
    blePeripheral.setAdvertisedServiceUuid(heartRateService.uuid()); // add the service UUID
    blePeripheral.addAttribute(heartRateService); // Add the BLE Heart Rate service
    blePeripheral.addAttribute(gyroRateChar); // add the Heart Rate Measurement characteristic

    /* Now activate the BLE device. It will start continuously transmitting BLE
    advertising packets and will be visible to remote BLE central devices
    until it receives a new connection */
    blePeripheral.begin();
    Serial.println("Bluetooth device active, waiting for connections...");
    /****************** END OF BLE HEARTRATE SETUP **************************/

    Serial.println("SETUP COMPLETE");

}

void loop() {
    // listen for BLE peripherals to connect:
    BLECentral central = blePeripheral.central();

    // if a central is connected to peripheral:
    if (central) {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());
        // turn on the LED to indicate the connection:
        digitalWrite(13, HIGH);

        // check the heart rate measurement every 200ms
        // as long as the central is still connected:
        while (central.connected()) {
            long currentMillis = millis();
            // if 200ms have passed, check the heart rate measurement:
            if (currentMillis - previousMillis >= 200) {
                previousMillis = currentMillis;
                updateGyroValue();
            }
        }
        // when the central disconnects, turn off the LED:
        digitalWrite(13, LOW);
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
    }
}

float getGZ() {
    int gxRaw, gyRaw, gzRaw; // raw gyro values
    float gx, gy, gz;

    // read raw gyro measurements from device
    CurieIMU.readGyro(gxRaw, gyRaw, gzRaw);

    // convert the raw gyro data to degrees/second
    //gx = convertRawGyro(gxRaw);
    //gy = convertRawGyro(gyRaw);
    gz = convertRawGyro(gzRaw);

    return gz;
}

void updateGyroValue() {
    /* Read the current voltage level on the A0 analog input pin.
    This is used here to simulate the heart rate's measurement.
     */
    int gryoMeasurement = abs(getGZ());
    Serial.print("Gyro Value:");
    Serial.println(gryoMeasurement);

    int gyroRate = map(gryoMeasurement, 0, 255, 0, 100);
    if (gyroRate != oldgyroRate || true /*OVERRIDING IF*/) { // if the heart rate has changed
        Serial.print("Mapped Value: "); // print it
        Serial.println(gyroRate);
        const unsigned char gyroRateCharArray[2] = {0, (char) gyroRate};
        gyroRateChar.setValue(gyroRateCharArray, 2); // and update the heart rate measurement characteristic
        oldgyroRate = gyroRate; // save the level for next comparison
    }
}

float convertRawGyro(int gRaw) {
    // since we are using 250 degrees/seconds range
    // -250 maps to a raw value of -32768
    // +250 maps to a raw value of 32767

    float g = (gRaw * 250.0) / 32768.0;

    return g;
}
