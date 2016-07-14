/*
  Author: Jose Nunez (jose.nunez@intel.com)
  This is Open Sourece Code, released under the terms of the "MIT License"
  Copyright Intel Coportation 2016
*/

#define board_led            13
#define left_speed_pin        5
#define right_speed_pin       6
#define left_direction_pin   10
#define right_direction_pin  11
#define MOTOR_LEFT           "L"
#define MOTOR_RIGHT          "R"
#define MOTOR_DIRECTION_FORWARD  1
#define MOTOR_DIRECTION_BACKWARDS 0

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(board_led, OUTPUT);
  pinMode(left_speed_pin, OUTPUT);
  pinMode(right_speed_pin, OUTPUT);
  pinMode(left_direction_pin, OUTPUT);
  pinMode(right_direction_pin, OUTPUT);

  
  doBlink(10, 100);

  rotateLeft();
  delay(3000);
  rotateRight();
  delay(3000);

  doBlink(3, 500);
}

// the loop function runs over and over again forever
void loop() {
  moveForward();
  doBlink(4,500);
  stopMotors();
  doBlink(3, 50);
  moveBackwards();
  doBlink(2,100);
  rotateLeft();
  doBlink(2, 250);
  
  moveForward();
  doBlink(3,500);
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
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_FORWARD, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, 100);  
}

void moveBackwards(){
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_BACKWARDS, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_BACKWARDS, 100);  
}

void rotateRight(){
  setMotor(MOTOR_LEFT,  MOTOR_DIRECTION_FORWARD, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_BACKWARDS, 100);  
}
void rotateLeft(){
  setMotor(MOTOR_LEFT,  MOTOR_DIRECTION_BACKWARDS, 100);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, 100);  
}

void stopMotors(){
  setMotor(MOTOR_LEFT, MOTOR_DIRECTION_FORWARD, 0);
  setMotor(MOTOR_RIGHT, MOTOR_DIRECTION_FORWARD, 0);  
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

