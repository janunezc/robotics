// This example depends on Paul Stoffregen's SerialFlash library
// Download at https://github.com/PaulStoffregen/SerialFlash

#include <CurieSerialFlash.h>
#include <SPI.h>

void setup() {
  //uncomment these if using Teensy audio shield
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7

  //uncomment these if you have other SPI chips connected
  //to keep them disabled while using only SerialFlash
  //pinMode(4, INPUT_PULLUP);
  //pinMode(10, INPUT_PULLUP);

  Serial.begin(9600);

  // wait for Arduino Serial Monitor
  while (!Serial) ;
  delay(100);
  Serial.println("All Files on SPI Flash chip:");

  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
    error("Unable to access SPI Flash chip");
  }

  SerialFlash.opendir();
  unsigned int count = 0;
  while (1) {
    char filename[64];
    unsigned long filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
      Serial.print("  ");
      Serial.print(filename);
      spaces(20 - strlen(filename));
      Serial.print("  ");
      Serial.print(filesize);
      Serial.print(" bytes");
      Serial.println();


  // Open the file
  SerialFlashFile ff = SerialFlash.open(filename);
  Serial.print("Opening file: ");
  Serial.println(filename);

  // Get the size and position
  uint32_t sz = ff.size();
  Serial.print("Size: "); 
  Serial.println(sz);
  
  uint32_t pos = ff.position();

  Serial.print("Address: ");
  Serial.println(pos);
  ff.seek(pos);

  // Read content of the file into a buffer
  char mybuff[sz];
  ff.read(mybuff, sz);

  Serial.print("|||");
  Serial.print(mybuff);
  Serial.println("|||");
  

    } else {
      break; // no more files
    }
  }
}

void spaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

void loop() {
}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
