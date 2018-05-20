#include <DynamixelSerial.h>

#define DYMX_ID 3
#define SCAN_MAX_ANGLE 900
#define SCAN_SPEED 10
#define SCAN_SIZE  20


int i;

int scan_angle;
int scan_direction;

void setup(){

     Serial.begin(57600);

  Dynamixel.begin(1000000,2);  // Initialize the servo at 1Mbps and Pin Control 2

  
  Dynamixel.setEndless(DYMX_ID, OFF);
  scan_angle=0;
  scan_direction=SCAN_SPEED;

  delay(1000);
  
}


void loop(){
    if (scan_angle> SCAN_MAX_ANGLE) 
    {
      scan_direction=-SCAN_SPEED;
    } 
    else 
    {
      if (scan_angle<-SCAN_MAX_ANGLE) 
      {
        scan_direction= SCAN_SPEED;
      }
    }
  scan_angle+=scan_direction;
  Serial.println(scan_angle);
  
  Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
 delay (20); 
}
