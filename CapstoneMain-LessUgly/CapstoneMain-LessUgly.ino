#include <SoftwareSerial.h>
#include <Servo.h>




  //PinNumDefinitions:
  //---------------HC-SR04---------------//
  const int trigPin = 22;
  const int echoPin = 24;
  long duration;
  int distance;
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
  Servo leftServo;  //create servo objects to control servos; left
  Servo rightServo; //right
  int pos = 0;    //variable to store the servo position
  //---------------Global---------------//
  String lastMessageReceived = "";          //Saves the most recently used message that we received from the ESP
  const String cipSend = "AT+CIPSEND=0,";   //Used for sending raw plaintext messages serially across ESP
  const String crlf = "\r\n";
  //EndPinDefinitions;
  
  
  
  
  
  //PinNumInstantiations
  ////---------------GPS---------------//
  SoftwareSerial GPS (rxPin, txPin);
  //---------------SERVO---------------//
  

void setup() {
  Serial.begin(9600); // Starts the serial communication
  Serial2.begin(9600);
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
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  
  //---------------GPS---------------//
  GPS.begin(9600);
  //---------------ESP---------------//
  initESP();
  delay(100);
  Serial.println("\nParsing incoming data:\n");
  //----------------Servo------------//
  leftServo.attach(2);  //attaches the servo on pin 2 to the servo object
  rightServo.attach(3); //attaches the servo on pin 3 to the servo object
}

void loop() {
  //Serial.println(receiveMessage(Serial, Serial2));
  parseReceiveString(receiveMessage(Serial, Serial2), Serial, Serial2);
}

void moveServo(int angle){
  pos = angle;
  leftServo.write(pos);              //tell servo to go to position in variable 'pos'
  rightServo.write(pos);             //tell servo to go to position in variable 'pos'
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
  int posInString = 0;
  int colonPos;
  String trimmedString = "";
  
  while(posInString < toTrim.length()-1){
    if(toTrim[posInString] == ':'){
      colonPos = posInString;
      break;
    }
    posInString++;
  }
  for(int i = colonPos + 1; i < toTrim.length()-1; i++){
    trimmedString += toTrim[i];
  }
  
  return trimmedString;
}

void parseReceiveString(String messageReceived, HardwareSerial &serialToMonitor, HardwareSerial &serialFromESP){
  int msgLength = messageReceived.length();
  String firstThreeChars;
  if(msgLength > 5){
    messageReceived = trimString(messageReceived);
    //next three lines build our new string....probably can be done in a much better way, but it works and is relatively quick.
    firstThreeChars = messageReceived[0];
    firstThreeChars.concat(messageReceived[1]);
    firstThreeChars.concat(messageReceived[2]);

          if(firstThreeChars.equals("FWD")){     //Case 1: Forward + Speed
          serialToMonitor.println("FWD");
    }else if(firstThreeChars.equals("LFT")){     //Case 2: Left (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       serialToMonitor.println("LFT");
    }else if(firstThreeChars.equals("RGT")){     //Case 3: Right (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       serialToMonitor.println("RGT");
    }else if(firstThreeChars.equals("BRK")){     //Case 4: Engage brakes; Bring car velocity to 0.
       serialToMonitor.println("BRK");
    }else if(firstThreeChars.equals("LOC")){     //Case 5: Send back GPS coordinates to the computer
      serialToMonitor.println("LOC");
      sendMessage("LOC", Serial2);
      sendMessage(getGPSData(), Serial2);
    }else if(firstThreeChars.equals("BCK")){     //Case 6: doDonuts = true;
      serialToMonitor.println("BCK");
    }else if(firstThreeChars.equals("DNT")){     //Case 6: doDonuts = true;
      serialToMonitor.println("DNT");
    }else if(firstThreeChars.equals("SON")){     //Case 7: Sonar (designed for multiple sonars
      serialToMonitor.println("SON");
      /*if(messageReceived[3] == 0){          //UseSonarEntity0
        String distance = "" + getSonar(trigPin, echoPin);
        sendMessage("SON0", Serial2);
        sendMessage(distance, Serial2);
      } */
    }else if(firstThreeChars == "RSV"){     //Case 7: Empty Case
      
    }
  }
}

long getSonar(int enable, int echo){
  // Clears the enable
  digitalWrite(enable, LOW);
  delayMicroseconds(2);
  
  // Sets the enable on HIGH state for 10 micro seconds
  digitalWrite(enable, HIGH);
  delayMicroseconds(10);
  digitalWrite(enable, LOW);
  
  // Reads the echo, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  
  // Calculating the distance
  distance = duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  
  return distance;
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

void stop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
  Serial.println("Stop");
}

