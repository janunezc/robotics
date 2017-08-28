// This example depends on Paul Stoffregen's SerialFlash library
// Download at https://github.com/PaulStoffregen/SerialFlash

#include <CurieSerialFlash.h>
#include <SPI.h>

#define FSIZE 256
#define ledPin 13

const char *filename = "rfidble.txt";

void setup() {
  Serial.begin(9600);
  debugMessage("Setup() BEGIN!");
  pinMode(ledPin, OUTPUT);
  parpadear (3,1000);
  delay(1000);// wait for Arduino Serial Monitor
  parpadear (3,500);
  
  // Init. SPI Flash chip
  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
    Serial.println("Unable to access SPI Flash chip");
  }  

  debugMessage("Setup() DONE!");
}

bool executedOnce = false;
void loop() {
  if(!executedOnce){  
    String fileContent = readFile();
    
    debugMessage("Loop() Result: --" + fileContent + "--");
    int len = fileContent.length();
    debugMessage("Loop() Length: " + String(len));

    if(!verifyBegin(fileContent) || !verifyEnd(fileContent) || !verifySearch(fileContent)){
      debugMessage("Loop() Verifications Failed. Reformatting...");
      removeFile();
      initFile();
      executedOnce = false;
    } else {
      executedOnce = true;
    }
  }

  parpadear (2,200);
  delay(1000);
}


bool create_if_not_exists (const char *filename) {
  if (!SerialFlash.exists(filename)) {
    Serial.println("Creating file " + String(filename));
    return SerialFlash.create(filename, FSIZE);
  }

  Serial.println("File " + String(filename) + " already exists");
  return true;
}

/**
 * THis function initializes file with raw content.
 */
void initFile(){
  debugMessage("initFile() BEGIN!");

  const char *contents = "FILE_BEGIN_OK:123455,7598635,----------,----------,----------,----------,----------,----------,----------,----------,----------,----------,FILE_END_OK";
  SerialFlashFile file;

  debugMessage("initFile() Creating file if not exist...");
  // Create the file if it doesn't exist
  if (!create_if_not_exists(filename)) {
    Serial.println("Not enough space to create file " + String(filename));
    digitalWrite(ledPin, HIGH);
    return;
  }

  debugMessage("initFile() Opening file for write!");
  // Open the file and write test data
  file = SerialFlash.open(filename);
  parpadear (3,50);
  
  debugMessage("initFile() Writing into file...!");
  file.write(contents, strlen(contents) + 1);
  debugMessage("String \"" + String(contents) + "\" written to file " + String(filename));
  
  debugMessage("initFile() Closing file...");

  file.close();

  debugMessage("initFile() DONE!");
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
  Serial.println();
  
  return result;
}

bool findKey(String key){
  debugMessage("FindKey(): " + key);
  String fileContent = readFile();
  int pos = fileContent.indexOf(key);
  if(pos==-1) {
    debugMessage("FindKey() Not found: " + key);
    return false;
  } else {
    debugMessage("FindKey() FOUND: " + key + " at " + String(pos));
    return true;
  }
}

void addKey (String key){
  debugMessage("AddKey() BEGIN!");
  String fileContent = readFile();
  if(findKey(key)== true){
    debugMessage("AddKey() Key already stored!");
    return; //Already there
  } else {
    int pos = fileContent.indexOf(",----------");
    String beginning = fileContent.substring(0,pos);
    String thisKey = "," + key + dashesForKey(key);
    String restOfFile = fileContent.substring(pos+11);
    String newFileContent = beginning + thisKey + restOfFile;
    debugMessage("OLD FILE:" + fileContent);
    debugMessage("NEW FILE:" + newFileContent);
    saveFile(newFileContent);
  }
}

void saveFile(String newFileContent){
  debugMessage("saveFile() BEGIN!");

  const char *contents = "FILE_BEGIN_OK:123455,7598635,----------,----------,----------,----------,----------,----------,----------,----------,----------,----------,FILE_END_OK";
  
  SerialFlashFile file;

  debugMessage("initFile() Creating file if not exist...");
  // Create the file if it doesn't exist
  if (!create_if_not_exists(filename)) {
    Serial.println("Not enough space to create file " + String(filename));
    digitalWrite(ledPin, HIGH);
    return;
  }

  debugMessage("initFile() Opening file for write!");
  // Open the file and write test data
  file = SerialFlash.open(filename);
  parpadear (3,50);

  debugMessage("initFile() Writing into file...!");
  file.write(contents, strlen(newFileContent) + 1);
  debugMessage("String \"" + String(contents) + "\" written to file " + String(filename));
  
  debugMessage("initFile() Closing file...");

  file.close();

  debugMessage("initFile() DONE!");

}

String dashesForKey(String key){
  int dashesRequired = 10-key.length();
  String result = "";
  for(int i=0; i<dashesRequired; i++){
    result = result + "-";
  }
  return result;
}
void removeKey(String key){
  
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

bool verifyBegin(String fileContent){
  if(fileContent.startsWith("FILE_BEGIN_OK")){
    debugMessage("FILE BEGIN IS OK!");
    return true;
  } else {
    debugMessage("ERROR!!!! FILE BEGIN IS WRONG");
    return false;
  }
}
bool verifyEnd(String fileContent){
  if(fileContent.endsWith("FILE_END_OK")){
    debugMessage("FILE END IS OK!");
    return true;
  } else {
    debugMessage("ERROR!!!! FILE END IS WRONG");
    return false;
  }  
}
bool verifySearch(String fileContent){
  int pos = fileContent.indexOf("7598635");
  int pos2 = fileContent.indexOf("notfoundhere");
  if(pos > 0 && pos2==-1) {
    debugMessage("SEARCH IS OK!");
    return true;
  } else {
    debugMessage("ERROR!!!! SEARCH IS WRONG!!");
    debugMessage(String(pos));
    debugMessage(String(pos2));
    return false;
  }
}

void removeFile(){
  debugMessage("removeFile() BEGIN!");

  debugMessage("removeFile() Removing file...");
  SerialFlash.remove(filename);
  parpadear (3,50);

  debugMessage("removeFile() DONE!");
}

