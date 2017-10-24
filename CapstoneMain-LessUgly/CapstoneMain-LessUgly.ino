#include <SoftwareSerial.h>
#include <Servo.h>




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
  boolean doDonuts = false;
  //EndPinDefinitions;
  
  
  
  
  
  //PinNumInstantiations
  ////---------------GPS---------------//
  SoftwareSerial GPS (rxPin, txPin);
  //---------------SERVO---------------//
  

void setup() {
  Serial.begin(9600); // Starts the serial communication
  Serial2.begin(9600);
  Serial2.setTimeout(3); //this sets the timeout for Serial.readBytesUntil(), Serial.readBytes(), Serial.parseInt() or Serial.parseFloat() methods.
  //---------------HC-SR04---------------//
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  double duration;
  double distance;
  //------------------ 4 Motors-----------------------
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
  //Serial.println(receiveMessage(Serial, Serial2));
 // parseReceiveString(receiveMessage(Serial, Serial2), Serial, Serial2);
  //constantly writing out a pwm at 255 to enable the motors all the time
 // analogWrite(ENA, 255);
 // analogWrite(ENB, 255);
  //moveServo(90);
  getSonar();
  delay(200);
}

void moveServo(int angle){
  pos = angle;
  sonarServo.write(pos);              //tell servo to go to position in variable 'pos'
  delay(15);                         //waits 15ms for the servo to reach the position
}

String receiveMessage(HardwareSerial &serialToMonitor, HardwareSerial &serialFromESP){
  lastMessageReceived;           //resets the global variable to be an empty string
  lastMessageReceived = serialFromESP.readStringUntil('\n');
  return lastMessageReceived;
}

void sendMessage(String message, HardwareSerial &serialToESP){
  String customCIPSend;
  customCIPSend = cipSend + message.length();   //cipSend is "AT+CIPSEND=0," So we need to add message length
  customCIPSend += crlf;
  message += crlf;
  
  for(int j = 0; j < customCIPSend.length()-1; j++){
    serialToESP.write(customCIPSend[j]);
  }
  
  for(int i = 0; i < message.length()-1; i++){
    serialToESP.write(message[i]);
  }
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

void parseReceiveString(String messageReceived, HardwareSerial &serialToMonitor, HardwareSerial &serialFromESP){
  String firstThreeChars;
  if(messageReceived.length() > 2){
    Serial.println(messageReceived);
  }
  if(messageReceived.length() > 5){
    messageReceived = trimString(messageReceived);

    firstThreeChars = messageReceived[0];
    firstThreeChars += messageReceived[1];
    firstThreeChars += messageReceived[2];

          if(firstThreeChars.equals("FWD")){     //Case 1: Forward
          serialToMonitor.println("FWD");
          goForward();
    }else if(firstThreeChars.equals("LFT")){     //Case 2: Left (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       serialToMonitor.println("LFT");
       goLeft();
    }else if(firstThreeChars.equals("RGT")){     //Case 3: Right (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       serialToMonitor.println("RGT");
       goRight();
    }else if(firstThreeChars.equals("BRK")){     //Case 4: Engage brakes; Bring car velocity to 0.
       serialToMonitor.println("BRK");
       roverStop();
    }else if(firstThreeChars.equals("LOC")){     //Case 5: Send back GPS coordinates to the computer
      serialToMonitor.println("LOC");
      sendMessage("LOC", Serial2);
      sendMessage(getGPSData(), Serial2);
    }else if(firstThreeChars.equals("BCK")){     //Case 6: Backward
      serialToMonitor.println("BCK");
      goBack();
    }else if(firstThreeChars.equals("DNT")){     //Case 7: doDonuts = true;
      serialToMonitor.println("DNT");
      while(doDonuts){
        //roverDonut();
        if(doDonuts){
          doDonuts = false;
        }
      }
    }else if(firstThreeChars.equals("SON")){     //Case 8: Sonar (Need to implement servo for looking left,str8,right
      serialToMonitor.println("SON");
      /*if(messageReceived[3] == 0){          //UseSonarEntity0
        String distance = "" + getSonar(trigPin, echoPin);
        sendMessage("SON0", Serial2);
        sendMessage(distance, Serial2);
      } */
    }else if(firstThreeChars == "RSV"){     //Case 9: Empty Case; Reserved
      
    }
  }
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

void goForward(){
  digitalWrite(in1,LOW);//digital output
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("Forward");
}

void goBack(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("Back");
}

void goLeft(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("Left");
}

void goRight(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("Right");
}

void roverStop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Serial.println("Stop");
}


