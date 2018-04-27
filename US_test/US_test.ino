#include "Arduino.h"
#include "ultrasonic.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
int distance;

//if there is a bottle between the bottle and the obstacle and the robot is at a distance of 50cm from the obstacle it will see th bottle
// --> use the 2 sensors in the front to be sure that this is not going on

void loop() {
  // put your main code here, to run repeatedly:
  distance = readUltrasonic(A5);
  Serial.println(distance);
  
}
