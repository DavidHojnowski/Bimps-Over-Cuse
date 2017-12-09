#include <Adafruit_GPS.h>
#include <Servo.h>
#include <Math.h>



//---------------Pin assignments and Global Variables---------------//
//---------------HC-SR04 Sonar---------------//
const int trigPin = 22;   //Physical PWM pin for sonar communication
const int echoPin = 24;   //Physical PWM pin for sonar communication
double duration;            //double that stores the time between pulses
double distance;             //Distance returned by getSonar function
//------------Pins for 4 motors-----------------//
//4 pins two for each channel
//left motors
int in1 = 26;   //Physical pin for motors
int in2 = 28;   //Physical pin for motors
//right motors
int in3 = 27;   //Physical pin for motors
int in4 = 29;   //Physical pin for motors
//The PWM bits for the 2 channels
int ENA = 2;   //Physical pin for motors
int ENB = 3;   //Physical pin for motors
//---------------GPS---------------//
const int rxPin = 52; //Physical pin for receiving data from GPS
const int txPin = 53; //Physical pin for sending data to GPS
double initialCoordsLat = 00.00;  //initial Latitude of our rover
double initialCoordsLong = 00.00; //initial Longitude of our rover
double currentCoordsLat = 00.00;  //current Latitude of our rover
double currentCoordsLong = 00.00;  //current Longitude of our rover
double finalCoordsLat = 43.049272;  //destination latitude -- Grewen's Clock tower
double finalCoordsLong = -76.087518; //destination longitude -- Grewen's Clock tower
double angleTowardDest = 0.0;       //angle that gives us the direction from initial point to final point
double angleRoverIsFacing = 0.0;    //angle we're facing
boolean currentCoordsSet = false;   //have we set currentCoords?
boolean initialCoordsSet = false;   //have we set initialCoords?
#define GPSECHO false               //needed for GPS code
#define GPSSerial Serial3           //initialize GPS to Serial bus #3
Adafruit_GPS GPS(&GPSSerial);
//---------------SERVO---------------//
Servo sonarServo;  //create servo objects to control servos; left
int pos = 0;    //variable to store the servo position
//---------------COMMUNICATION---------------//
String lastMessageReceived = "";          //Saves the most recently used message that we received from the ESP
const String cipSend = "AT+CIPSEND=0,";   //Used for sending raw plaintext messages serially across ESP

//---------------MISC Globals---------------//
#define pi   3.14159265358979323846264338327950288    //Pi for math problems
boolean automatic = false;                            //Automation boolean -- changes on ATM/MAN command
boolean pause = false;                                //pausing boolean -- changes based on PSE command
uint32_t timer = millis();                            //timer for GPS querying
int checkDirection = 0;                               //counter for checking where we are vs. where we want to be
boolean facingCorrectDirection = false;               //name explains it
//---------------End Pin assignments and Global Variables---------------//










//---------------Required setup() function---------------//
//Pre-Conditions: Initialize or call initializations for each component
//Post-Conditions: All components have been initialized
void setup() {
  automatic = false;
  pause = false;
  Serial.begin(9600); // Starts the serial communication
  Serial2.begin(9600);
  Serial2.setTimeout(3); //this sets the timeout for Serial.readBytesUntil(), Serial.readBytes(), Serial.parseInt() or Serial.parseFloat() methods.
  //---------------HC-SR04---------------//
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  //---------------Motors---------------//
  pinMode(in1,OUTPUT);  //Pin init for motor controls
  pinMode(in2,OUTPUT);  //Pin init for motor controls
  pinMode(in3,OUTPUT);  //Pin init for motor controls
  pinMode(in4,OUTPUT);  //Pin init for motor controls
  pinMode(ENA,OUTPUT);  //Pin init for motor controls
  pinMode(ENB,OUTPUT);  //Pin init for motor controls
  //the PWM's can have a range of 0-255
  analogWrite(ENA, 255);  //Pin init for motor controls
  analogWrite(ENB, 255);  //Pin init for motor controls
  
  //---------------GPS---------------//
  GPS.begin(9600); //intialize serial at a baud rate of 9600
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);      //Initialization command for GPS module
   // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);      //Initialization command for GPS module
  setInitialCoordinates();                        //Queries GPS to get initial coords
  //---------------ESP---------------//
  initESP();              //
  delay(400);
  Serial.println("\nParsing incoming data:\n");
  //----------------Servo------------//
  sonarServo.attach(4);  //attaches the servo on pin 4 to the servo object
  moveServo(90);          //moves Servo to forward point
}
//---------------End required setup() function---------------//





//---------------Main/required loop() function---------------//

//Pre-Conditions: Main routine; Constantly writes two motor pins to max number. Moves in straight lines to from current position to final position.
//Post-Conditions: Arrived at final position.
void loop() {
  parseReceiveString(receiveMessage());
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  while(automatic){
    parseReceiveString(receiveMessage());
    roverFaceTowardDestination();
    while(getSonar() > 50){
      parseReceiveString(receiveMessage());
      analogWrite(ENA, 255);
      analogWrite(ENB, 255);
      roverMoveForward();
      if(!automatic)
        break;
      checkDirection++;
      if(checkDirection == 1073741824){ //at 16MHz processor, we'll reach this approximately every 60 seconds
        checkDirection = 0;
        roverFaceTowardDestination();
      }
    }
    checkDirection++;
    if(checkDirection == 1073741824){//at 16MHz processor, we'll reach this approximately every 60 seconds
      checkDirection = 0;
      roverFaceTowardDestination();
    }
  }
}
//---------------End Main and Required loop() function---------------//










//---------------Mathematics Functions---------------//

//Pre-Conditions: Compare current coordinates to our destination's coordinates and find the angle needed to face our destination
//Post-Conditions: Angle toward destination from current position has been determined and returned.
double determineNeededRoverDirection(){
  double differenceLat = finalCoordsLat - currentCoordsLat;
  double differenceLong = cos(pi / 180 * currentCoordsLat) * (finalCoordsLong - currentCoordsLong); //the reason this isnt just final - current is because longitude is weird and has to be scaled by lat
  double angleNeeded = atan2(differenceLong, differenceLat);
  return angleNeeded;
}

//Pre-Conditions: Have coordinates in unusable format Decimal Minutes. Need to convert to usable format Decimal Degrees.
//Post-Conditions: Decimal Minutes converted to Decimal Degrees and returned.
double convertToDecDegFromDecMin(double decMinsCoord){
  double decDegsCoord = 0.0;
  int numDegrees = decMinsCoord / 100;
  double minutes = decMinsCoord - (numDegrees*100);
  minutes = minutes/60.0;
  decDegsCoord = numDegrees + minutes;
  return decDegsCoord;
}
//---------------End Mathematics Functions---------------//










//---------------Communication Functions---------------//

//Pre-Conditions: Receives message from the ESP module through the Serial2 bus
//Post-Conditions: Message returned
String receiveMessage(){
  lastMessageReceived = "";           //resets the global variable to be an empty string
  lastMessageReceived = Serial2.readStringUntil('\n');
  return lastMessageReceived;
}

//Pre-Conditions: We have a message ready and need to send it across the socket using AT commands and a CRLF at the end
//Post-Conditions: Message sent across socket to Java code
void sendMessage(String message){
  String customCIPSend;
  customCIPSend = cipSend + message.length();   //cipSend is "AT+CIPSEND=0," So we need to add message length
  Serial2.println();
  delay(100);
  Serial2.println(customCIPSend);
  delay(100);
  Serial2.println(message);
  delay(100);
}

//Pre-Conditions: A string has been received and we need to trim the header data from it to parse
//Post-Conditions: String has been trimmed and can now be parsed
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


//Pre-Conditions: Does a little bit of string manipulation but then handles what we've received in the form of our protocol
//Post-Conditions: Protocol command has been handles appropriately
String parseReceiveString(String messageReceived){
  String receivedCommand;
 // Serial.println(messageReceived);
  if(messageReceived.length() > 5){
    messageReceived = trimString(messageReceived);
    
    receivedCommand = messageReceived[0];
    receivedCommand += messageReceived[1];
    receivedCommand += messageReceived[2]; //Makes sure we only have 3 letter commands

    if(!pause){
      if(receivedCommand.equals("LFT") && !automatic){     //Case 1: Left
              roverTurnLeft();
      }else if(receivedCommand.equals("RGT") && !automatic){     //Case 2: Right
        roverTurnRight();
      }else if(receivedCommand.equals("FWD") && !automatic){     //Case 3: Forward
        roverMoveForward();
      }else if(receivedCommand.equals("BCK") && !automatic){     //Case 4: Backward
        roverMoveBackward();
      }else if(receivedCommand.equals("BRK") && !automatic){     //Case 5: Engage brakes; Bring car velocity to 0.
        roverStop();
      }else if(receivedCommand.equals("DNT") && !automatic){     //Case 6: doDonuts = true;
        roverDoDonuts();
      }else if(receivedCommand.equals("LOC")){     //Case 7: Send back GPS coordinates to the computer
        sendLocation();
      }else if(receivedCommand.equals("PSE")){     //Case 8: Pause the rover's movement
        pause = !pause;
      }else if(receivedCommand.equals("MAN")){     //Case 9: Enables manual control
        automatic = false;
      }else if(receivedCommand.equals("ATM")){     //Case 10: Enables automatic control
        automatic = true;
      }else if(receivedCommand.equals("RET")){     //Case 11: Return to starting position
        int tempLat, tempLong;
        tempLat = initialCoordsLat;
        tempLong = initialCoordsLong;
        initialCoordsLong = finalCoordsLong;
        initialCoordsLat = finalCoordsLat;
        finalCoordsLat = tempLat;
        finalCoordsLong = tempLong;
      }else if(receivedCommand.equals("RSV")){     //Case 12: Empty Case; Reserved
      }
    }else if(receivedCommand.equals("PSE")){     //Unpause the rover's movement
            pause = !pause;
    }
  }
  return receivedCommand;
}

//Pre-Conditions: We need to format the location data a bit before sending to the java code
//Post-Conditions: Formatted coordinates and sent to Java code
void sendLocation(){
  char tempLat[8];
  char tempLong[8];
  // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
  dtostrf(currentCoordsLat, 4, 2, tempLat);
  dtostrf(currentCoordsLong*-1, 4, 2, tempLong);
  String latitude = String(tempLat);
  String longitude = String(tempLong);
  String formatForSend = latitude + "N, " + longitude + "W";
  sendMessage(formatForSend);
}

//---------------End Communication Functions---------------//










//---------------Component Control Functions---------------//

//Pre-Conditions: Moves servo motor to input angle
//Post-Conditions: servo is now in position param(angle)
void moveServo(int angle){
  pos = angle;
  sonarServo.write(pos);              //tell servo to go to position in variable 'pos'
  delay(15);                         //waits 15ms for the servo to reach the position
}

//Pre-Conditions: Continuously Queries the GPS during initialization so we know where we started
//Post-Conditions: Initial coords are set and we continue with initialization
void setInitialCoordinates(){
  while(!initialCoordsSet){
    getGPSData(0);
  }
  
}

//typeOfCoords = 0 for initial coords
//typeOfCoords = 1 for current coords

//Pre-Conditions: Need to read from the GPS and output to the Serial Monitor(console) so we know what is happening. Also sets curr/initial coords.
//Post-Conditions:  Initial/curr coords are set and data is output to Serial Monitor
void getGPSData(int typeOfCoords){
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
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
      if(typeOfCoords == 1){
        currentCoordsLat = convertToDecDegFromDecMin(GPS.latitude);
      }else if(typeOfCoords == 0){
        initialCoordsLat = convertToDecDegFromDecMin(GPS.latitude);
      }
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      if(typeOfCoords == 1){
        currentCoordsLong = convertToDecDegFromDecMin(GPS.longitude) * -1;
      }else if(typeOfCoords == 0){
        initialCoordsLong = convertToDecDegFromDecMin(GPS.longitude) * -1;
      }
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
        if(typeOfCoords == 1){
        if(currentCoordsLat > 0.0){
          currentCoordsSet = true;
        }
      }
      if(typeOfCoords == 0){
        if(initialCoordsLat > 0.0){
          initialCoordsSet = true;
        }
      }
}

//Pre-Conditions: Need to get distance from the sonar sensor and convert it into centimeters
//Post-Conditions: Retrieval of distance and conversion accomplished and returned.
double getSonar(){
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
  return distance;
}

//Pre-Conditions: Need to send AT commands that initialize our ESP module correctly 
//Post-Conditions: ESP module is now correctly configured and we can now connect to it appropriately
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

//Pre-Conditions: Attempt to face in the direction of double value angleTowardDestination
//Post-Conditions: Facing in the correct direction and we are now able to drive straight
void roverFaceTowardDestination(){
  currentCoordsLat = 0.0;
  currentCoordsLong = 0.0;
  while(automatic){
    while(!facingCorrectDirection){
      currentCoordsSet = false;
      while(!currentCoordsSet){
        getGPSData(1);
       }
      int count = 2500;//THIS LINE WILL LIKELY NEED SOME REFINING
      while(count > 0){//THIS LINE WILL LIKELY NEED SOME REFINING
        roverMoveForward();
        count--;
      }
      currentCoordsSet = false;
      while(!currentCoordsSet){
        getGPSData(1);
      }
      angleRoverIsFacing = GPS.angle;
      angleTowardDest = determineNeededRoverDirection();
    
      if((angleRoverIsFacing < (angleTowardDest + 45)) && ((angleRoverIsFacing) > (angleTowardDest - 45))){
        facingCorrectDirection = true;
      }else{
        for(int i = 0; i < 300; i++){//THIS LINE WILL LIKELY NEED SOME REFINING
        roverTurnRight();
        }
        Serial.println("Failed");
        facingCorrectDirection = false;
      }
      parseReceiveString(receiveMessage());
      if(!automatic){
        break;
      }
    }
  }
}

//Pre-Conditions: Moves rover forward
//Post-Conditions: Rover has moved as long as the associated key has been held down
void roverMoveForward(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

//Pre-Conditions: Moves rover backward
//Post-Conditions: Rover has moved as long as the associated key has been held down
void roverMoveBackward(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

//Pre-Conditions: Turns rover left
//Post-Conditions: Rover has moved as long as the associated key has been held down
void roverTurnLeft(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

//Pre-Conditions: Turns rover right
//Post-Conditions: Rover has moved as long as the associated key has been held down
void roverTurnRight(){
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

//Pre-Conditions: Moves applies brakes and stops motion
//Post-Conditions: Rover stopped
void roverStop(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

//Pre-Conditions: Does donuts (for fun purposes only)
//Post-Conditions: Does donuts (for fun purposes only)
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


