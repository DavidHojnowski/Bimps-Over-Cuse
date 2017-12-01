#include <Adafruit_GPS.h>
#include <Servo.h>



  //Pin assignments and Global Variables
  //---------------HC-SR04 Sonar---------------//
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
  const int rxPin = 52; //may need to flip these 2 values
  const int txPin = 53;
  double initialCoordsLat = 0000.00;  //initial Latitude of our rover
  double initialCoordsLong = 0000.00; //initial Longitude of our rover
  double finalCoordsLat;  //destination latitude
  double finalCoordsLong; //destination longitude
  boolean initialCoordsSet;
  uint32_t timer = millis();
  #define GPSECHO false
  #define GPSSerial Serial3
  Adafruit_GPS GPS(&GPSSerial);
  //---------------SERVO---------------//
  Servo sonarServo;  //create servo objects to control servos; left
  int pos = 0;    //variable to store the servo position
  //---------------Global---------------//
  String lastMessageReceived = "";          //Saves the most recently used message that we received from the ESP
  const String cipSend = "AT+CIPSEND=0,";   //Used for sending raw plaintext messages serially across ESP
  const String crlf = "\r\n";
  #define pi   3.14159265358979323846264338327950288
  boolean automatic = false;
  boolean pause = false;
  String commandReceived;

  //EndPinDefinitions;
  

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
  GPS.begin(9600); //intialize serial at a baud rate of 9600
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
   // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 
  getGPSData();
  initialCoordsLat = GPS.latitude;
  initialCoordsLong = GPS.longitude;
  //---------------ESP---------------//
  initESP();
  delay(400);
  Serial.println("\nParsing incoming data:\n");
  //----------------Servo------------//
  sonarServo.attach(4);  //attaches the servo on pin 4 to the servo object
  moveServo(90);  
}

void loop() {
  if(!initialCoordsSet){  //this line is saying check GPS until we get our initial coords, then move on. Otherwise, we get timing issues.
  getGPSData();
  }else{
    commandReceived = parseReceiveString(receiveMessage());
    if(!pause){
      analogWrite(ENA, 255);
      analogWrite(ENB, 255);
      //getSonar();
      while(automatic){
        parseReceiveString(receiveMessage());
        //doAutoThings();
      }
    }
  }
}


//---------------Mathematics Functions---------------//



//---------------End Mathematics Functions---------------//


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
  int posInString = 0;  //Since we know the message will have "+IPD=x,y:"
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
          //sendMessage(initialCoordsLat);
          //sendMessage(initialCoordsLong);
    }else if(receivedCommand.equals("PSE")){     //Case 8: Pause/Unpause the rover's movement
          pause = !pause;
    }else if(receivedCommand.equals("MAN")){     //Case 9: Enables manual control, only if paused
          if(pause){
            automatic = false;
          }
    }else if(receivedCommand.equals("ATM")){     //Case 10: Enables automatic control, only if paused
          if(pause){
            automatic = true;
          }
    }else if(receivedCommand.equals("RET")){     //Case 11: Return to starting position
      
    }else if(receivedCommand.equals("RSV")){     //Case 12: Empty Case; Reserved
      
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

void getGPSData(){
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); Serial.print(':');
    Serial.print(GPS.minute, DEC); Serial.print(':');
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      initialCoordsLat = GPS.latitude;
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      initialCoordsLong = GPS.longitude;
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      if(initialCoordsLong > 0.0){
        initialCoordsSet = true;
      }
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


