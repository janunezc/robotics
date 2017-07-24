// This example depends on Paul Stoffregen's SerialFlash library
// Download at https://github.com/PaulStoffregen/SerialFlash

#include <CurieSerialFlash.h>
#include <SPI.h>

#define FSIZE 256
#define ledPin 13

const char *filename = "myfile2.txt";
const char *contents = "1234567890ABCDEFGHIJKL";

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  parpadear (3,1000);
  // wait for Arduino Serial Monitor
  delay(1000);
  parpadear (5,200);
  // Init. SPI Flash chip
  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
    Serial.println("Unable to access SPI Flash chip");
    digitalWrite(ledPin, HIGH);
  }

  SerialFlashFile file;

  // Create the file if it doesn't exist
  if (!create_if_not_exists(filename)) {
    Serial.println("Not enough space to create file " + String(filename));
    digitalWrite(ledPin, HIGH);
    return;
  }

  // Open the file and write test data
  file = SerialFlash.open(filename);
  parpadear (3,100);
  file.write(contents, strlen(contents) + 1);
  Serial.println("String \"" + String(contents) + "\" written to file " + String(filename));
  file.close();

  parpadear (3,100);
  // Open the file
  SerialFlashFile ff = SerialFlash.open(filename);
  Serial.print("Opening file: ");
  Serial.println(filename);

  // Get the size and position
  uint32_t sz = 21;//ff.size();
  Serial.print("Size: "); 
  Serial.println(sz);
  parpadear (3,100);
  
  uint32_t pos = ff.position();

  Serial.print("Address: ");
  Serial.println(pos);
  ff.seek(pos);

  // Read content of the file into a buffer
  char mybuff[sz];
  ff.read(mybuff, sz);

  ff.close();

  Serial.print("|||");
  Serial.print(String(mybuff));
  Serial.println("|||");
  parpadear (10,50);
}

bool create_if_not_exists (const char *filename) {
  if (!SerialFlash.exists(filename)) {
    Serial.println("Creating file " + String(filename));
    return SerialFlash.create(filename, FSIZE);
  }

  Serial.println("File " + String(filename) + " already exists");
  return true;
}

void loop() {
}

void parpadear(int times, int milliseconds){
  for(int i=0; i<times; i++){
    digitalWrite(ledPin, HIGH);
    delay(milliseconds);
    digitalWrite(ledPin, LOW);
    delay(milliseconds);
  }
}

