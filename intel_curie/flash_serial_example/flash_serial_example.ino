// Escrito por José Núñez como un ejemplo didáctico para el dominio público.
// Utiliza la librería SerialFlash de Paul Stoffregen.
// Dicha librería se puede descargar de https://github.com/PaulStoffregen/SerialFlash

//Abstracción de la librería SerialFlash para el controlador Intel Curie
#include <CurieSerialFlash.h>
#include <SPI.h>

#define FSIZE 256 //Tamaño predefinido del archivo
#define ledPin 13 //PIN para un LED 
#define buttonPin 10 //PIN para leer un botón. Debe estar "pull-up" y el botón realiza una conexión a tierra.

const char *filename = "rfidble.txt"; //Nombre del archivo
#define CONTENT_SIZE 153 //Tamaño predefinido del contenido del archivo para este ejemplo.

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  parpadear (5, 1000);

  debugMessage("Setup() BEGIN!");

  parpadear (3, 500);
  
  // Inicializar operación de la librería SerialFlash
  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
    debugMessage("Setup() Unable to access SPI Flash chip");
  }

  debugMessage("Setup() DONE!");
}
/*------*/
/* LOOP */
/*------*/
bool executedOnce = false;
bool addKeyMode = false;
void loop() {
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
  
  processButtonCase();

}

void processButtonCase() {
  heartBeat();

  checkButton();

  if (addKeyMode) {
    addKey("1234");
    addKey("5678");
    addKey("91011");
    addKey("121314");
    addKey("151617");
    addKeyMode = false;
  }
}

unsigned long nextHeartBeat = 0;
void heartBeat() {
  if (millis() > nextHeartBeat) {
    debugMessage("Heartbeat!");
    parpadear (2, 200);
    nextHeartBeat = millis() + 2000;
  }
}

/******************************
   INTERACTION LAYER
 ******************************/

bool didResetMode = false;
void checkButton() {
  if (digitalRead(buttonPin) == LOW) {
    debugMessage("checkButton() Button is low!");
    long lowDetectionMillis = millis();

    while (digitalRead(buttonPin) == LOW) {
      long timeInLow = millis() - lowDetectionMillis;
      if (timeInLow > 3000) {
        debugMessage("checkButton() button was low for more than 3 seconds! Initializing File...");
        initFile();
        didResetMode = true;
        break;
      }
    }
    if (!didResetMode) {
      debugMessage("checkButton() Switching to addKey mode...");
      addKeyMode = true;
    }

    didResetMode = false;
  }
}

/*****************************
   DEBUG LAYER
 *****************************/
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

bool verifySize(String fileContent) {
  debugMessage("verifySize() BEGIN!");
  if (fileContent.length() == CONTENT_SIZE) {
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


/******************************************
   BUSINESS LAYER
 ******************************************/

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


/********************
   DATA ACCESS LAYER
 ********************/

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
  debugMessage("Size:" + String(result.length()));

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
  while (!create_if_not_exists(filename)) {
    debugMessage("Memory filled up! Clearing Flash...");
    SerialFlash.eraseAll();

    while (SerialFlash.ready() == false) {
      // wait, 30 seconds to 2 minutes for most chips
    }
  }

  debugMessage("saveFile() Opening file for write!");
  // Open the file and write test data
  file = SerialFlash.open(filename);

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
    debugMessage("Creating file " + String(filename));
    return SerialFlash.create(filename, FSIZE);
  }

  debugMessage("File " + String(filename) + " already exists");
  return true;
}
