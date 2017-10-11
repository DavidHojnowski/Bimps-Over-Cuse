//Hardware Serial pins TX2/RX2

void setup() {
  Serial.begin(115200);                                              
  Serial2.begin(115200);

  initESP();

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

void loop() {
  while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  
  while (Serial.available() > 0) {
    Serial2.write(Serial.read());
  }
  

}






