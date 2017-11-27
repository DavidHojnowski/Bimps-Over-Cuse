#include <SoftwareSerial.h>

const byte rxPin = 52;
const byte txPin = 53;

// set up a GPS object
SoftwareSerial GPS (rxPin, txPin);


void setup() {
  Serial.begin(9600);
  
  GPS.begin(9600);
}

void loop() {
 // GPS.println();
    while(GPS.available())
  {
    char c = GPS.read();
    Serial.print(c);
  }
  //Serial.println("wow");
 // delay(500);
}
