/*
 * Programa escrito por Jose Nunez como ejemplo didáctico del uso del RDM6300 con un ARDUINO 101.
 * Use bajo su propio riesgo.
 * Ejemplo de dominio público.
 *
*/
#include "rdm630.h"

rdm630 rfid(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
int led_yellow_close = 2;
int led_green_open = 3;
int led_signal = 13;
long OPEN_PULSE_WIDTH = 800;
long CLOSE_PULSE_WIDTH = 1000;
long OPEN_STATE_DURATION = 5000; 
long CODE_READ_DELAY = 10000;
long codeReadDelayMaxMillis = millis();

void setup()
{
    Serial.begin(115200);  // start serial to PC
    pinMode(led_yellow_close, OUTPUT);
    pinMode(led_green_open, OUTPUT);
    pinMode(led_signal, OUTPUT);
    rfid.begin();
    ledSignal(5,500);
    setMessage("READY!");
}

void loop()
{
  unsigned long rfidTagCode = 0;  
  
  if(rfid.available()){
    setMessage("RFID Data is available! reading it...");
    rfidTagCode = readRFIDCode();
    setMessage("Data: " + String(rfidTagCode));
    if(millis() > codeReadDelayMaxMillis) {
      if(rfidTagCode == 7598635) {
        setMessage("OPEN");
        openPulse();//This involves a closePulse() call inside the openPulse() function.
        codeReadDelayMaxMillis = millis() + CODE_READ_DELAY;
      } else {
        setMessage("I DONT KNOW YOU!");
        closePulse();
      }
    } else {
      setMessage("SkippingNewReads");
      delay(500);
    }   
  }
}

void openPulse(){
  setMessage("SENDING OPEN PULSE");
  digitalWrite(led_green_open, HIGH);
  delay(OPEN_PULSE_WIDTH);
  digitalWrite(led_green_open,LOW);
  setMessage("OPEN PULSE DONE! Waiting for OPEN STATE DURATION...");
  delay(OPEN_STATE_DURATION);
  setMessage("OPEN STATE COMPLETE");
  closePulse();
}

void closePulse(){
  setMessage("SENDING CLOSE PULSE");
  digitalWrite(led_yellow_close, HIGH);
  delay(CLOSE_PULSE_WIDTH);
  digitalWrite(led_yellow_close, LOW);
  setMessage("CLOSE PULSE DONE!");
}

unsigned long readRFIDCode(){
  byte data[6];
  byte length;

  rfid.getData(data,length);
  Serial.println("Data valid");
  for(int i=0;i>length;i++){
      Serial.print(data[i],HEX);
      Serial.print(" ");
  }
  Serial.println();
  //concatenate the bytes in the data array to one long which can be 
  //rendered as a decimal number
  unsigned long result = 
    ((unsigned long int)data[1]<<24) + 
    ((unsigned long int)data[2]<<16) + 
    ((unsigned long int)data[3]<<8) + 
    data[4];              
  Serial.print("decimal CardID: ");
  Serial.println(result);
  return result;  
}

void ledSignal(int times, int milliseconds){
  for(int i=0; i<times; i++){
    digitalWrite(led_signal, HIGH);
    delay(milliseconds);
    digitalWrite(led_signal, LOW);
    delay(milliseconds);
  }
}

void addToMessage(String message){
  setMessage(message, false);
}

void setMessage(String message){
  setMessage(message, true);
}

void setMessage(String message, bool newLine){
  String timeStamp = String(millis());
  String finalMessage = timeStamp + " - " + message;
  if(newLine){
    Serial.println(finalMessage);
  } else {
    Serial.print(message);
  }
}

