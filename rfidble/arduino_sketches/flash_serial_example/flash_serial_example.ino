// Written by Jose Nunez as didactic example on pulic domain.
// This example depends on Paul Stoffregen's SerialFlash library
// Download at https://github.com/PaulStoffregen/SerialFlash

#include <CurieSerialFlash.h>
#include <SPI.h>

#define FSIZE 256
#define ledPin 13

const char *filename = "rfidble.txt";
#define CONTENT_SIZE 153

void setup() {
  Serial.begin(9600);
  debugMessage("Setup() BEGIN!");
  pinMode(ledPin, OUTPUT);
  parpadear (3, 1000);
  delay(1000);// wait for Arduino Serial Monitor
  parpadear (3, 500);

  // Init. SPI Flash chip
  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
    Serial.println("Unable to access SPI Flash chip");
  }

  debugMessage("Setup() DONE!");
}
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
bool executedOnce = false;
void loop() {
  debugMessage("loop() BEGIN!");
  if (!executedOnce) {
    debugMessage("loop() executing... reading file...");
    String fileContent = readFile();

    debugMessage("loop() file read: |" + fileContent + "| (" + String(fileContent.length()));

    debugMessage("loop() doing file verifications...");
    if (!verifyBegin(fileContent) || !verifyEnd(fileContent) || !verifySearch() || ! verifySize(fileContent)) {
      debugMessage("loop() Verifications Failed. Reformatting...");
      initFile();
      executedOnce = false;
    } else {
      debugMessage("loop() verifications ran OK");

      debugMessage("loop() testing key addition");
      test_addKey();

      debugMessage("loop() testing key removal");
      test_removeKey();
      executedOnce = true;
    }
  }

  parpadear (2, 200);
  delay(1000);
}

void test_addKey() {
  debugMessage("test_addKey() BEGIN!");
  String newKey = "2097876";
  debugMessage("test_addKey() Key to add: " + newKey);
  
  bool newKeyAlreadyFound = findKey(newKey);
  if (newKeyAlreadyFound) {
    debugMessage("test_addKey() WRONG: Key was not expected, but found. " + newKey);
  } else {
    debugMessage("test_addKey() CORRECT: New Key was not found, as expected. " + newKey);
  }

  debugMessage("test_addKey() Calling addKey()!");
  addKey(newKey);

  debugMessage("test_addKey() verifying addKey() results...");
  bool newKeyFound = findKey(newKey);
  if (newKeyFound) {
    debugMessage("test_addKey() CORRECT: New Key was found as expected. " + newKey);
  } else {
    debugMessage("test_addKey() WRONG: New Key was not found. " + newKey);
  }
}

void test_removeKey() {
  debugMessage("test_removeKey() BEGIN!");
  String keyToRemove = "2097876";
  debugMessage("test_removeKey() Key to remove: " + keyToRemove);
  
  bool keyToRemovePresent = findKey(keyToRemove);
  if (keyToRemovePresent) {
    debugMessage("test_removeKey() CORRECT: Key to remove is present as expected. " + keyToRemove);
  } else {
    debugMessage("test_removeKey() WRONG: Key to remove is not found!!!. " + keyToRemove);
  }

  debugMessage("test_removeKey() calling remove key...");
  removeKey(keyToRemove);

  if (findKey(keyToRemove)) {
    debugMessage("test_removeKey() WRONG: Key to remove is present. NOT EXPECTED!!!! " + keyToRemove);
  } else {
    debugMessage("test_removeKey() CORRECT: Key to remove not present as expected.");
  }

}
void parpadear(int times, int milliseconds) {
  for (int i = 0; i < times; i++) {
    if(times > 1) debugMessage("Heartbeat!");
    digitalWrite(ledPin, HIGH);
    delay(milliseconds);
    digitalWrite(ledPin, LOW);
    delay(milliseconds);
  }
}

void debugMessageNoLF(String message) {
  Serial.print (message);
}

void debugMessage(String message) {
  long curMillis = millis();
  Serial.print (curMillis);
  Serial.print (": ");
  Serial.println (message);
}

bool verifyBegin(String fileContent) {
  debugMessage("verifyBegin() BEGIN!");
  if (fileContent.startsWith("FILE_BEGIN_OK")) {
    debugMessage("verifyBegin() CORRECT: File begin is OK");
    return true;
  } else {
    debugMessage("verifyBegin() WRONG: File begin is NOT AS EXPECTED");
    return false;
  }
}
bool verifyEnd(String fileContent) {
  debugMessage("verifyEnd() BEGIN!");
  if (fileContent.endsWith("FILE_END_OK")) {
    debugMessage("verifyEnd() Correct: End of file is as expected");
    return true;
  } else {
    debugMessage("verifyEnd() WRONG: End of file is NOT AS EXPECTED");
    return false;
  }
}

bool verifySize(String fileContent){
  debugMessage("verifySize() BEGIN!");
  if(fileContent.length()==CONTENT_SIZE){
    debugMessage("verifySize() CORRECT!");
    return true;
  } else {
    debugMessage("verifySize() WRONG!");
    return false;
  }
}
bool verifySearch() {
  debugMessage("verifySearch() BEGIN!");
  bool k1found = findKey("7598635");
  bool k2found = findKey("notfoundhere");

  if (k1found && ! k2found) {
    debugMessage("verifySearch() SEARCH IS OK!");
    return true;
  } else {
    debugMessage("verifySearch() ERROR!!!! SEARCH IS WRONG!!");
    return false;
  }
}




bool findKey(String key) {
  debugMessage("findKey(): " + key);
  String formattedKey = formatKey(key);
  String fileContent = readFile();
  int pos = fileContent.indexOf(formattedKey);
  if (pos > 0) {
    debugMessage("findKey() FOUND: " + key + " at " + String(pos));
    return true;
  } else {
    debugMessage("findKey() Not found: " + key);
    return false;
  }
}

void addKey (String key) {
  debugMessage("AddKey() BEGIN!");
  String fileContent = readFile();
  if (findKey(key) == true) {
    debugMessage("AddKey() Key already stored!");
    return; //Already there
  } else {
    int pos = fileContent.indexOf(",----------");
    String beginning = fileContent.substring(0, pos);
    String thisKey = formatKey(key);
    String restOfFile = fileContent.substring(pos + 11);
    String newFileContent = beginning + thisKey + restOfFile;
    debugMessage("OLD FILE:" + fileContent);
    debugMessage("NEW FILE:" + newFileContent);
    saveFile(newFileContent);
  }
}

String formatKey(String key) {

  int dashesRequired = 10 - key.length();
  String result = "";
  for (int i = 0; i < dashesRequired; i++) {
    result = result + "-";
  }
  return "," + key + result;
}
void removeKey(String key) {
  debugMessage("removeKey() BEGIN!");
  String fileContent = readFile();
  if (findKey(key) == false) {
    debugMessage("removeKey() key not present!");
    return; //Already not there
  } else {
    String formattedKey = formatKey(key);
    debugMessage("removeKey() Formatted Key: |" + formattedKey + "|");
    int pos = fileContent.indexOf(formattedKey);
    debugMessage("removeKey() Index of Formatted key: " + String(pos));
    if (pos > 0) {
      String beginning = fileContent.substring(0, pos);
      String thisKey = formatKey("");
      String restOfFile = fileContent.substring(pos + 11);
      String newFileContent = beginning + thisKey + restOfFile;
      debugMessage("removeKey() OLD FILE:" + fileContent);
      debugMessage("removeKey() NEW FILE:" + newFileContent);
      saveFile(newFileContent);
    } else {
      debugMessage("removeKey() Formatted Key not found! " + formattedKey);
    }
  }

}





/**
   This function initializes file with raw content.
*/
void initFile() {
  debugMessage("initFile() BEGIN!");

  String fileContent = "FILE_BEGIN_OK:123455,7598635---,----------,----------,----------,----------,----------,----------,----------,----------,----------,----------,FILE_END_OK";
  saveFile(fileContent);

  debugMessage("initFile() DONE!");
}

/**
   This function reads file content into an string. Implements full debugging mechanism
*/
String readFile() {
  debugMessage("readFile(): Begin!");

  debugMessage("readFile() Opening file: " + String(filename));
  SerialFlashFile file = SerialFlash.open(filename);

  // Get the size and position
  uint32_t fsize = file.size();
  debugMessage("readFile() Size: " + String(fsize));

  uint32_t filePos = file.position();
  debugMessage("readFile() Position: " + String(filePos));

  debugMessage("readFile() Seeking...");
  file.seek(filePos);

  debugMessage("readFile() Reading buffer...");
  char mybuff[fsize];
  file.read(mybuff, fsize);
  String result = String(mybuff);
  debugMessage("readFile() Closing file...");
  file.close();

  debugMessage("readFile() Content:");
  debugMessageNoLF("|||");
  debugMessageNoLF(result);
  debugMessageNoLF("|||");
  Serial.println("Size:" + String(result.length()));

  debugMessage("readFile() DONE!");
  return result;
}

/**
   This function drops existing file and saves a new one with proposed content.
*/
void saveFile(String newFileContent) {
  debugMessage("saveFile() BEGIN!");
  removeFile();
  uint8_t flashBuffer[CONTENT_SIZE + 1];
  newFileContent.getBytes(flashBuffer, CONTENT_SIZE + 1);
  SerialFlashFile file;

  debugMessage("saveFile() Creating file if not exist...");
  // Create the file if it doesn't exist
  if (!create_if_not_exists(filename)) {
    Serial.println("Not enough space to create file " + String(filename));
    digitalWrite(ledPin, HIGH);
    return;
  }

  debugMessage("saveFile() Opening file for write!");
  // Open the file and write test data
  file = SerialFlash.open(filename);
  parpadear (3, 50);

  debugMessage("saveFile() Writing into file...!");
  file.write(flashBuffer, CONTENT_SIZE + 1 );
  debugMessage("saveFile() String \"" + String(newFileContent) + "\" written to file " + String(filename));

  debugMessage("saveFile() Closing file...");

  file.close();

  debugMessage("saveFile() DONE!");

}

/**
   This function drops the file.
*/
void removeFile() {
  debugMessage("removeFile() BEGIN!");

  if (SerialFlash.exists(filename)) {
    debugMessage("removeFile() Removing file...");
    SerialFlash.remove(filename);
  } else {
    debugMessage("removeFile() No file to remove..." + String(filename));
  }
  parpadear (3, 50);

  debugMessage("removeFile() DONE!");
}

bool create_if_not_exists (const char *filename) {
  if (!SerialFlash.exists(filename)) {
    Serial.println("Creating file " + String(filename));
    return SerialFlash.create(filename, FSIZE);
  }

  Serial.println("File " + String(filename) + " already exists");
  return true;
}
