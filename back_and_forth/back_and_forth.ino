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
  delay(1000);
  //Serial1.begin(9600);
  delay(1000);
  
  // initialize digital pin 13 as an output.
  pinMode(board_led, OUTPUT);
  pinMode(left_speed_pin, OUTPUT);
  pinMode(right_speed_pin, OUTPUT);
  pinMode(left_direction_pin, OUTPUT);
  pinMode(right_direction_pin, OUTPUT);


  doBlink(10, 100);
  
  //Serial1.println("SETUP_COMPLETE!"); 
}

// the loop function runs over and over again forever
void loop() {
  //Serial1.print("ACC_FW");
  accelerate(MOTOR_DIRECTION_FORWARD);
  delay(2000);
  //Serial1.print("SLO_FW");
  slowDown(MOTOR_DIRECTION_FORWARD);
  delay(1000);

  //Serial1.print("ACC_BW");
  accelerate(MOTOR_DIRECTION_BACKWARDS);
  delay(2000);
  //Serial1.print("SLO_BW");
  slowDown(MOTOR_DIRECTION_BACKWARDS);
  delay(1000);
}

void accelerate(int motorDirection){
    for(int motorSpeed = 0; motorSpeed <100; motorSpeed += 10){
      setMotor(MOTOR_RIGHT, motorDirection, motorSpeed);
      setMotor(MOTOR_LEFT, motorDirection, motorSpeed);
    }   
}

void slowDown( int motorDirection){
    for(int motorSpeed = 100; motorSpeed >0; motorSpeed -=10){
      setMotor(MOTOR_LEFT, motorDirection, motorSpeed);
      setMotor(MOTOR_RIGHT,motorDirection, motorSpeed);
    }  
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

//USE THIS AS HUMANCOMMUNICATION MECHANISM 
void doBlink(int times, int delayTime){
  for(int i=0; i<times; i++){
    digitalWrite(board_led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(delayTime);              // wait for a second
    digitalWrite(board_led, LOW);    // turn the LED off by making the voltage LOW
    delay(delayTime);              // wait for a second    
  }
}
