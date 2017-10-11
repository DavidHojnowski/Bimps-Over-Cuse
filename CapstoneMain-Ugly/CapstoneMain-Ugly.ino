#include <SoftwareSerial.h>
#include <Servo.h>




  //PinNum&GlobalVariableDefinitions:
  //---------------HC-SR04---------------//
  const int trigPin = 22;
  const int echoPin = 24;
  long duration;
  int distance;
  //---------------GPS---------------//
  const int rxPin = 3;
  const int txPin = 1;
  //---------------SERVO---------------//
  Servo leftServo;  //create servo objects to control servos; left
  Servo rightServo; //right
  
  int pos = 0;    //variable to store the servo position
  //EndPinDefinitions;
  
  

  //PinNumInstantiations
  ////---------------GPS---------------//
  SoftwareSerial GPS (rxPin, txPin);
  //---------------SERVO---------------//
  

void setup() {
  Serial.begin(9600); // Starts the serial communication
  //---------------HC-SR04---------------//
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //---------------GPS---------------//
  GPS.begin(9600);
  //---------------ESP---------------//
  Serial.begin(115200);                                              
  Serial2.begin(115200);
  initESP();
  //--------------SERVO---------------//
  leftServo.attach(2);  //attaches the servo on pin 2 to the servo object
  rightServo.attach(3); //attaches the servo on pin 3 to the servo object

  
}

void loop() {
  //---------------HC-SR04---------------//
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  //---------------GPS---------------//
    while(GPS.available())
  {
    char c = GPS.read();
    Serial.print(c);
  }
  
  //---------------ESP---------------//
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  
  while (Serial.available() > 0) {
    Serial2.write(Serial.read());
  }

  //---------------SERVO---------------//
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

void initESP() {
  Serial.println("\nESP initialization beginning...");
  delay(50);
  Serial2.write("AT+CIPMUX=1\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);
  Serial2.write("AT+CWMODE=2\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);
  Serial2.write("AT+CIPSERVER=1\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);
  Serial2.write("AT+CIPAP=\"192.168.88.88\"\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);
  Serial2.write("AT+CWSAP=\"BlimpOverCuse\",\"blimp\",3,0\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);
  
  
  Serial.println("\nESP initialization complete."); 
}
