#include <Servo.h>

Servo leftServo;  //create servo objects to control servos; left
Servo rightServo; //right

int pos = 0;    //variable to store the servo position

void setup() {
  leftServo.attach(2);  //attaches the servo on pin 2 to the servo object
  rightServo.attach(3); //attaches the servo on pin 3 to the servo object
  
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {//goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    leftServo.write(pos);              //tell servo to go to position in variable 'pos'
    rightServo.write(pos);             //tell servo to go to position in variable 'pos'
    delay(15);                         //waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) {// goes from 180 degrees to 0 degrees
    leftServo.write(pos);              //tell servo to go to position in variable 'pos'
    rightServo.write(pos);             //tell servo to go to position in variable 'pos'
    delay(15);                         //waits 15ms for the servo to reach the position
  }
}

