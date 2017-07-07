#include <CurieBLE.h>

const int ledPin = 13; // set ledPin to on-board LED
//                                                        V
//ID IS A TEST. IT WAS 19B10010-E8F2-537E-4F6C-D104768A1214<<<<<
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1215"); // create service


// create switch characteristic and allow remote device to read and write
BLECharCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  // begin initialization
  BLE.begin();

  // set the local name peripheral advertises
  BLE.setLocalName("Blynk");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    Serial.println(ledCharacteristic.value());
    if (ledCharacteristic.value()) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }
}

