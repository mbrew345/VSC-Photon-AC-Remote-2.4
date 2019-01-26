// This #include statement was automatically added by the Particle IDE.
#include "VCNL4010.h"

VCNL4010 vcnl;

void setup() {
  Serial.begin(9600);
  Serial.println("VCNL4010 test");

  if (! vcnl.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.println("Found VCNL4010");
}


void loop() {
   Serial.print("Ambient: "); Serial.println(vcnl.readAmbient());
   Serial.print("Proimity: "); Serial.println(vcnl.readProximity());
   delay(1000);
}
