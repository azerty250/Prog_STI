#include <DynamixelSerial.h>

#define DYMX_ID 11
#define SCAN_MAX_ANGLE 1000
#define SCAN_SPEED 10
#define SCAN_SIZE  20
#define DYN2 5

int i;

int scan_angle;
int scan_direction;
int mode;


void setup(){

  Serial.begin(57600);

  Dynamixel.begin(1000000,2);  // Initialize the servo at 1Mbps and Pin Control 2

  mode = -1;
  Dynamixel.setEndless(DYMX_ID, OFF);
  Dynamixel.setEndless(DYN2, OFF);

  scan_angle=100;
  scan_direction=SCAN_SPEED;

  delay(1000);
  
}
 int error;


void loop()
{
  if(Serial.available())
  {
      char val = Serial.read();
      if(val== '0')
      {
        mode = 0;
      }
      else if(val=='1')
      {
        mode = 1;
      }
      else if(val == '2')
      {
        mode = 2;
      }
  }

  scan_angle+=scan_direction;
  
 if (scan_angle>= SCAN_MAX_ANGLE) 
 {
    scan_direction=-SCAN_SPEED;
  } 
  else if(scan_angle <=100)
  {
    scan_direction = SCAN_SPEED;
  }
      Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
    Dynamixel.moveSpeed(DYN2,(500-(6*scan_angle/18)),1000); 
  Serial.print(mode);
  Serial.print(" ");
  Serial.println(scan_angle);
  if(scan_angle == SCAN_MAX_ANGLE)
  {
        delay(4000);

  }
  else if(scan_angle == 100)
  {
    delay(4000);

  }

/*
  if(scan_angle < 850 && mode == 0)
  {
    Dynamixel.torqueStatus(DYMX_ID,0);
    Dynamixel.torqueStatus(DYN2,0);
    Dynamixel.ledStatus(DYN2,0);
    scan_angle+=SCAN_SPEED;
    Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
    Dynamixel.moveSpeed(DYN2,(500-(6*scan_angle/18)),1000);  
   // delay(500);
   }
  else if(mode == 1 && scan_angle > 20)
  {
    Dynamixel.torqueStatus(DYMX_ID,1);
    Dynamixel.torqueStatus(DYN2,1);
            Dynamixel.ledStatus(DYN2,1);

    scan_angle-=SCAN_SPEED;
    Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
    Dynamixel.moveSpeed(DYN2,(500-(6*scan_angle/18)),1000);  
    //delay(2000); 
  }
  */
 delay (20); 
}
