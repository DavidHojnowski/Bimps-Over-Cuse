
//#include <SoftwareSerial.h>

//SoftwareSerial swSer(50, 51, false);

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  Serial.println("\nSoftware serial test started");

  //for (char ch = ' '; ch <= 'z'; ch++) {
   // swSer.write(ch);
  //}
  //swSer.println("");

}

void loop() {
  while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  
  while (Serial.available() > 0) {
    Serial2.write(Serial.read());
  }
  

}
