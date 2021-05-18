#include<SoftwareSerial.h>
#include<TinyGPS++.h>

TinyGPSPlus gps;
SoftwareSerial mySerial(0,1);

void setup() {
  Serial.begin(57600);
  mySerial.begin(9800);
}

void loop() {
  char g=mySerial.read();
  gps.encode(g);
  if(gps.location.isUpdated()){
    Serial.println(gps.date.year());
  }
}
