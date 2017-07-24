// This example depends on Paul Stoffregen's SerialFlash library
// Download at https://github.com/PaulStoffregen/SerialFlash

#include <CurieSerialFlash.h>
#include <SPI.h>

#define FSIZE 256
#define ledPin 13

const char *filename = "rfidble.txt";
const char *contents = "1234567890ABCDEFGHIJKL";

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  parpadear (3,1000);
  delay(1000);// wait for Arduino Serial Monitor
  parpadear (3,500);
}

bool create_if_not_exists (const char *filename) {
  if (!SerialFlash.exists(filename)) {
    Serial.println("Creating file " + String(filename));
    return SerialFlash.create(filename, FSIZE);
  }

  Serial.println("File " + String(filename) + " already exists");
  return true;
}


bool executedOnce = false;
void loop() {
  if(!executedOnce){
    
    initFile();
    
    String fileContent = readFile();
    debugMessage("RESULT AT LOOP: --" + fileContent + "--");
    executedOnce = true;
  }

  parpadear (2,200);
  delay(500);
}

/*
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

*/

/**
 * THis function initializes file with raw content.
 */
void initFile(){
  debugMessage("initFile() BEGIN!");
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
}

/**
 * This function reads file content into an string. Implements full debugging mechanism
 */
String readFile(){
  debugMessage("readFile(): Begin!");
  
  debugMessage("Opening file: " + String(filename));
  SerialFlashFile file = SerialFlash.open(filename);

  // Get the size and position
  uint32_t fsize = file.size();
  debugMessage("Size: " + String(fsize)); 
  
  uint32_t filePos = file.position();
  debugMessage("Position: " + String(filePos));

  debugMessage("Seeking...");
  file.seek(filePos);

  debugMessage("Reading buffer...");
  char mybuff[fsize];
  file.read(mybuff, fsize);
  String result = String(mybuff);
  debugMessage("Closing file...");
  file.close();

  debugMessage("Here is what we found:");
  debugMessageNoLF("|||");
  debugMessageNoLF(result);
  debugMessageNoLF("|||");
  
  return result;
}

void initializeMemory(){
  
}

void parpadear(int times, int milliseconds){
  for(int i=0; i<times; i++){
    debugMessage("Heartbeat!");
    digitalWrite(ledPin, HIGH);
    delay(milliseconds);
    digitalWrite(ledPin, LOW);
    delay(milliseconds);
  }
}

void debugMessageNoLF(String message){
  Serial.print (message);
}

void debugMessage(String message){
  long curMillis = millis();
  Serial.print (curMillis);
  Serial.print (": ");
  Serial.println (message);
}

