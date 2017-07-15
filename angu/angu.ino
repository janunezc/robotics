#include <CurieBLE.h>

const int ledPin = 3; // set ledPin to on-board LED
//                                                        V
//ID IS A TEST. IT WAS 19B10010-E8F2-537E-4F6C-D104768A1214<<<<<
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1215"); // create service


// create switch characteristic and allow remote device to read and write
BLECharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite, 20);

void setup() {
  Serial.begin(9600);
  delay(500);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
  ledSignal(5,200);
  // begin initialization
  BLE.begin();

  // set the local name peripheral advertises
  BLE.setLocalName("ANGU");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add the service
  BLE.addService(ledService);
  
  //const char* valueToWrite = "THIS IS A TEXT VALUE";
  //ledCharacteristic.write(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
  
}

void loop() {
  // poll for BLE events
  BLE.poll();

  if (ledCharacteristic.written()) {
    // update LED, either central has written to characteristic or button state has changed
    Serial.println(ledCharacteristic.valueLength());
    const byte* val = ledCharacteristic.value();
    
    String valueStr = String((const char *)val).substring(0,ledCharacteristic.valueLength());
    Serial.println("---" + valueStr + "---");
    /**
    12345678901234567890 1234567890
    ON
    ON345678901234567890
    */
    if (valueStr.startsWith("ON")) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void ledSignal(int times, int milliseconds){
  for(int i=0; i<times; i++){
    digitalWrite(ledPin, HIGH);
    delay(milliseconds);
    digitalWrite(ledPin, LOW);
    delay(milliseconds);
  }
}
