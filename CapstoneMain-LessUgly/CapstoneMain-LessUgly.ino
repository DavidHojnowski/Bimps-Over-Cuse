#include <SoftwareSerial.h>
#include <Servo.h>




  //PinNumDefinitions:
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
  //---------------GPS---------------//
  GPS.begin(9600);
  //---------------ESP---------------//
  initESP();
  Serial.println("\nSoftware serial test started");
  //----------------Servo------------//
  leftServo.attach(2);  //attaches the servo on pin 2 to the servo object
  rightServo.attach(3); //attaches the servo on pin 3 to the servo object
}


void loop() {
  parseReceiveString(receiveMessage(Serial2, Serial1), Serial1, Serial2);
  delay(2000);
}

void moveServo(int angle){
  pos = angle;
  leftServo.write(pos);              //tell servo to go to position in variable 'pos'
  rightServo.write(pos);             //tell servo to go to position in variable 'pos'
  delay(15);                         //waits 15ms for the servo to reach the position
}

String receiveMessage(HardwareSerial &serialFromESP, HardwareSerial &serialToMonitor){
    lastMessageReceived = "";           //resets the global variable to be an empty string
  while (serialFromESP.available() > 0) {
    lastMessageReceived.concat(serialFromESP.read()); //builds the new string off the input sitting on the Serial2 pins
  }
  //sendMessage(lastMessageReceived, Serial);         //Writing what we receive from serial
  for(int i = 0; i<lastMessageReceived.length();i++){
    Serial.write(lastMessageReceived[i]);
  }
  return lastMessageReceived;
}

void sendMessage(String message, HardwareSerial &serialToESP){
  String customCIPSend;
  customCIPSend = cipSend + message.length();   //cipSend is "AT+CIPSEND=0," So we need to add message length
  customCIPSend += crlf;
  message += crlf;
  
  for(int j = 0; j < customCIPSend.length(); j++){
    serialToESP.write(customCIPSend[j]);
  }
  
  for(int i = 0; i < message.length(); i++){
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

void parseReceiveString(String messageReceived, HardwareSerial &HSer1, HardwareSerial &HSer2){
  int msgLength = messageReceived.length();
  String firstThreeChars;
  if(msgLength > 2){
    firstThreeChars = messageReceived[0] + messageReceived[1] + messageReceived[2];

          if(firstThreeChars == "FWD"){     //Case 1: Forward + Speed
          Serial.write("F");
    }else if(firstThreeChars == "LFT"){     //Case 2: Left (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       Serial.write("L");
    }else if(firstThreeChars == "RGT"){     //Case 3: Right (Might be preset turn or maybe we accept an angle and translate it to how to turn wheels to the left)
       Serial.write("R");
    }else if(firstThreeChars == "BRK"){     //Case 4: Engage brakes; Bring car velocity to 0.
       Serial.write("B");
    }else if(firstThreeChars == "LOC"){     //Case 5: Send back GPS coordinates to the computer
        sendMessage("LOC", Serial2);
        sendMessage(getGPSData(), Serial2);
    }else if(firstThreeChars == "DNT"){     //Case 6: doDonuts = true;

    }else if(firstThreeChars == "SON"){     //Case 7: Sonar (designed for multiple sonars
      if(messageReceived[3] == 0){          //UseSonarEntity0
        String distance = "" + getSonar(trigPin, echoPin);
        sendMessage("SON0", Serial2);
        sendMessage(distance, Serial2);
      } 
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

