/*
  Author: Jose Nunez (jose.nunez@intel.com)
  This is Open Sourece Code, released under the terms of the "MIT License"
  Copyright Intel Coportation 2016
*/

#define board_led            13
#define left_speed_pin        5
#define right_speed_pin       6
#define servo01_pin           9
#define left_direction_pin   10
#define right_direction_pin  11
#define proximity_echo        2
#define proximity_trigger    12
#define MOTOR_LEFT           "L"
#define MOTOR_RIGHT          "R"
#define MOTOR_DIRECTION_FORWARD  1
#define MOTOR_DIRECTION_BACKWARDS 0
#define FORWARD_TIME          10000
#define TOO_CLOSE_DISTANCE    6
#define MOTOR_RIGHT_FW_SPEED 91
#define MOTOR_LEFT_FW_SPEED  100
#define MOTOR_RIGHT_BW_SPEED 91
#define MOTOR_LEFT_BW_SPEED  100
#define ROTATE_TIME 150

#include <Servo.h>

Servo servo1;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Setting pin modes...");
  pinMode(board_led, OUTPUT);
  pinMode(left_speed_pin, OUTPUT);
  pinMode(right_speed_pin, OUTPUT);
  pinMode(left_direction_pin, OUTPUT);
  pinMode(right_direction_pin, OUTPUT);

  pinMode(servo01_pin, OUTPUT);
  servo1.attach(servo01_pin);
  servo1.write(180);
  delay(1000);
  servo1.write(0);

  pinMode(proximity_trigger, OUTPUT);
  pinMode(proximity_echo, INPUT);
  
  Serial.println("Initial blink...");
  doBlink(10, 100);

  rotateLeft();
  delay(3000);
  rotateRight();
  doBlink(3, 500);

  
  Serial.println("SETUP COMPLETE");
}

long limitMillis = 0;
int distance = 0;
int tooCloseDetected = 0;
// the loop function runs over and over again forever
void loop() {
  limitMillis = millis() + FORWARD_TIME;
  moveForward();
  
  tooCloseDetected = 0;
  Serial.println("waiting for FORWARD_TIME or TOO_CLOSE_DISTANCEx3");
  while (millis() < limitMillis){
    distance = getDistance();
    if(distance < TOO_CLOSE_DISTANCE && distance > 0){
      Serial.println("Too Close Detected!");
      tooCloseDetected ++;
      if(tooCloseDetected>=3) {
        Serial.println("Too Close Detected 3 times!");
        break;
      }
    }
  }

  stopMotors();
  doBlink(3, 50);
  moveBackwards();
  doBlink(2,100);
  rotateLeft();
  doBlink(2, ROTATE_TIME);
  
  limitMillis = millis() + FORWARD_TIME;
  moveForward();
  
  tooCloseDetected = 0;
  Serial.println("waiting for FORWARD_TIME or TOO_CLOSE_DISTANCEx3");
  while (millis() < limitMillis){
    distance = getDistance();
    if(distance < TOO_CLOSE_DISTANCE && distance > 0){
      Serial.println("Too Close Detected!");
      tooCloseDetected++ ;
      if(tooCloseDetected>=3) {
        Serial.println("Too Close Detected 3 times!");
        break;
      }
    }
  }
   
  stopMotors();
  doBlink(3, 50);
  moveBackwards();
  doBlink(2,100);
  rotateRight();
  doBlink(2, 250);
}

/* 
 * motor: "L" for left and "R" for Right
 * direction: 1 for Forward, 0 for Backwards
 * speed: percentage 0-100 for speed of the motor
 */

void setMotor(String motor, int motorDirection, int motorSpeed){
  if(motor==MOTOR_RIGHT){
    digitalWrite(right_direction_pin, motorDirection);
    analogWrite(right_speed_pin, motorSpeed);
  }

  if(motor==MOTOR_LEFT){
    digitalWrite(left_direction_pin, motorDirection);
    analogWrite(left_speed_pin, motorSpeed);
  }
}

void moveForward(){
  servo1.write(90);
  Serial.println("Moving FORWARD...");
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_FORWARD, MOTOR_LEFT_FW_SPEED);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, MOTOR_RIGHT_FW_SPEED);  
}

void moveBackwards(){
  servo1.write(90);
  Serial.println("Moving BACKWARDS...");
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_BACKWARDS, MOTOR_LEFT_BW_SPEED);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_BACKWARDS, MOTOR_RIGHT_BW_SPEED);  
}

void rotateRight(){
  servo1.write(0);
  Serial.println("Rotating RIGHT...");
  setMotor(MOTOR_LEFT,  MOTOR_DIRECTION_FORWARD, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_BACKWARDS, 100);  
}
void rotateLeft(){
  servo1.write(180);
  Serial.println("Rotating LEFT...");
  setMotor(MOTOR_LEFT,  MOTOR_DIRECTION_BACKWARDS, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, 100);  
}

void stopMotors(){
  servo1.write(90);
  Serial.println("STOPPING MOTORS...");
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_FORWARD, 0);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, 0);  
}

// Returns proximity distance in CM
int getDistance(){
  digitalWrite(proximity_trigger, HIGH);
  delay(15);
  digitalWrite(proximity_trigger,LOW);
  int duration = pulseIn(proximity_echo, HIGH);
  int distance = (duration/2) / 29.1;  
  return distance;
}

//USE THIS AS HUMANCOMMUNICATION MECHANISM 
void doBlink(int times, int delayTime){
  for(int i=0; i<times; i++){
    digitalWrite(board_led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(delayTime);              // wait for a second
    digitalWrite(board_led, LOW);    // turn the LED off by making the voltage LOW
    delay(delayTime);              // wait for a second    
  }
}

