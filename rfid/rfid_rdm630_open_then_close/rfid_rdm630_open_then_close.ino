#include "rdm630.h"

rdm630 rfid(6, 0);  //TX-pin of RDM630 connected to Arduino pin 6
int led_yellow = 2;
int led_green = 3;
long OPEN_PULSE_WIDTH = 800;
long CLOSE_PULSE_WIDTH = 1000;
long OPEN_STATE_DURATION = 5000; 
long CODE_READ_DELAY = 10000;
long codeReadDelayMaxMillis = millis();

void setup()
{
    Serial.begin(9600);  // start serial to PC
    pinMode(led_yellow, OUTPUT);
    pinMode(led_green, OUTPUT);
    rfid.begin();
    ledSignal(5,150);
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
      if(rfidTagCode == 3716290) {
        setMessage("OPEN");
        openPulse();
        closePulse();
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
  digitalWrite(led_green, HIGH);
  delay(OPEN_PULSE_WIDTH);
  digitalWrite(led_green,LOW);
  setMessage("OPEN PULSE DONE! Waiting for OPEN STATE DURATION...");
  delay(OPEN_STATE_DURATION);
  setMessage("OPEN STATE COMPLETE");
}

void closePulse(){
  setMessage("SENDING CLOSE PULSE");
  digitalWrite(led_yellow, HIGH);
  delay(CLOSE_PULSE_WIDTH);
  digitalWrite(led_yellow, LOW);
  setMessage("CLOSE PULSE DONE!");
}

unsigned long readRFIDCode(){
  byte data[6];
  byte length;

  rfid.getData(data,length);
  Serial.println("Data valid");
  for(int i=0;i<length;i++){
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
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, HIGH);
    delay(milliseconds);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
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

