#include <Servo.h>
#include <SoftwareSerial.h>



  //PinNumDefinitions:
  //---------------HC-SR04---------------//
  const int trigPin = 22;
  const int echoPin = 24;
  //------------Pins for 4 motors-----------------//
  //4 pins two for each channel
  //left motors
  int in1 = 26;
  int in2 = 28;
  //right motors
  int in3 = 27;
  int in4 = 29;
  //The PWM bits for the 2 chanesl
  int ENA = 2;
  int ENB = 3;
  //---------------GPS---------------//
  const int rxPin = 3;
  const int txPin = 1;
  //---------------SERVO---------------//
  Servo sonarServo;  //create servo objects to control servos; left
  int pos = 0;    //variable to store the servo position
  //---------------Global---------------//
  String lastMessageReceived = "";          //Saves the most recently used message that we received from the ESP
  const String cipSend = "AT+CIPSEND=0,";   //Used for sending raw plaintext messages serially across ESP
  const String crlf = "\r\n";
  long duration;
  int distance;
  boolean automatic;
  boolean pause;
  String commandReceived;
  //EndPinDefinitions;
  
  
  
  
  
  //PinNumInstantiations
  //---------------GPS---------------//
  SoftwareSerial GPS (rxPin, txPin);
  //---------------SERVO---------------//
  

void setup() {
  automatic = false;
  pause = false;
  Serial.begin(9600); // Starts the serial communication
  Serial2.begin(9600);
  Serial2.setTimeout(3); //this sets the timeout for Serial.readBytesUntil(), Serial.readBytes(), Serial.parseInt() or Serial.parseFloat() methods.
  //---------------HC-SR04---------------//
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  double duration;
  double distance;
  //---------------Motors---------------//
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  //the PWM's can have a range of 0-255
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  
  //---------------GPS---------------//
  GPS.begin(9600);
  //---------------ESP---------------//
  initESP();
  delay(400);
  Serial.println("\nParsing incoming data:\n");
  //----------------Servo------------//
  sonarServo.attach(4);  //attaches the servo on pin 4 to the servo object
}

void loop() {
  commandReceived = parseReceiveString(receiveMessage());
  if(!pause){
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
  
    
    //getSonar();
    //delay(200);
    while(automatic){
      parseReceiveString(receiveMessage());
      //doAutoThings();
    }
  }
}


//---------------Communication Functions---------------//

String receiveMessage(){
  lastMessageReceived = "";           //resets the global variable to be an empty string
  lastMessageReceived = Serial2.readStringUntil('\n');
  return lastMessageReceived;
}

void sendMessage(String message){
  String customCIPSend;
  customCIPSend = cipSend + message.length();   //cipSend is "AT+CIPSEND=0," So we need to add message length
  customCIPSend += crlf;
  message += crlf;
  
  for(int j = 0; j < customCIPSend.length()-1; j++){
    Serial2.write(customCIPSend[j]);
  }
  
  for(int i = 0; i < message.length()-1; i++){
    Serial2.write(message[i]);
  }
}

String trimString(String toTrim){  //Trims the header data off of the received messages
  int posInString = 0;  //Since we know the message will have "+IPD=x,y:" we can just jump it further into our string to speed it up a bit
  int colonPos;
  int toTrimLength = toTrim.length();
  String trimmedString = "";
  
  while(posInString < toTrimLength-1){
    if(toTrim[posInString] == ':'){
      colonPos = posInString;
      break;
    }
    posInString++;
  }
  for(int i = colonPos + 1; i < toTrimLength-1; i++){
    trimmedString += toTrim[i];
  }
  
  return trimmedString;
}

String parseReceiveString(String messageReceived){
  String receivedCommand;
  if(messageReceived.length() > 5){
    messageReceived = trimString(messageReceived);
    
    receivedCommand = messageReceived[0];
    receivedCommand += messageReceived[1];
    receivedCommand += messageReceived[2];

          if(receivedCommand.equals("LFT")){     //Case 1: Left (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
            roverTurnLeft();
    }else if(receivedCommand.equals("RGT")){     //Case 2: Right (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
          roverTurnRight();
    }else if(receivedCommand.equals("FWD")){     //Case 3: Forward
          roverMoveForward();
    }else if(receivedCommand.equals("BCK")){     //Case 4: Backward
          roverMoveBackward();
    }else if(receivedCommand.equals("BRK")){     //Case 5: Engage brakes; Bring car velocity to 0.
          roverStop();
    }else if(receivedCommand.equals("DNT")){     //Case 6: doDonuts = true;
          roverDoDonuts();
    }else if(receivedCommand.equals("LOC")){     //Case 7: Send back GPS coordinates to the computer
          sendMessage(getGPSData());
    }else if(receivedCommand.equals("PSE")){     //Case 8: Pause the rover's movement
          pause = true;
    }else if(receivedCommand.equals("RES")){     //Case 9: Resume the rover's movement
          pause = false;
    }else if(receivedCommand.equals("MAN")){     //Case 10: Enables manual control, only if paused
          if(pause){
            automatic = false;
          }
    }else if(receivedCommand.equals("ATM")){     //Case 11: Enables automatic control, only if paused
          if(pause){
            automatic = true;
          }
    }else if(receivedCommand.equals("RET")){     //Case 12: Return to starting position
      
    }else if(receivedCommand.equals("RSV")){     //Case 13: Empty Case; Reserved
      
    }
  }

  return receivedCommand;
}

//---------------End Communication Functions---------------//

//---------------Component Control Functions---------------//

void moveServo(int angle){
  pos = angle;
  sonarServo.write(pos);              //tell servo to go to position in variable 'pos'
  delay(15);                         //waits 15ms for the servo to reach the position
}

String getGPSData(){
    String GPSData = ""; 
    while(GPS.available())
  {
    char c = GPS.read();
    GPSData += c;
    Serial.print(c);
  }
  return GPSData;
}

void getSonar(){
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
  Serial2.write("AT+CWSAP=\"RoverOnCuse\",\"rover\",3,0\r\n");
    while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  delay(400);

  Serial2.write("AT+ATE0");
      while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
    
  }
  
  Serial.println("\nESP initialization complete."); 
}

//---------------End Component Control Functions---------------//


//---------------Rover Movement Functions---------------//

void roverMoveForward(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void roverMoveBackward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void roverTurnLeft(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void roverTurnRight(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void roverStop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

void roverDoDonuts(){
  int count = 500;
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  
  while(count > 0){
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
    count--;
  }
}

//---------------End Rover Movement Functions---------------//


