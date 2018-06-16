#include "dyn_control.h"

void init_dyn()
{
	Dynamixel.begin(1000000, 2);  // Initialize the servo at 1Mbps and Pin Control 2

	Dynamixel.setEndless(DYMX_ID_LEFT, OFF);
	Dynamixel.setEndless(DYMX_ID_RIGHT, OFF);

	delay(10);
}

void open_gate()
{
    int angle = -200;
  while(angle != 600)
  {
      Dynamixel.moveSpeed(DYMX_ID_DOOR, (500 + (6 * angle / 18)), 1000);
      angle += 10;
        delay(20);
  }
  delay(100);
}

void close_gate()
{
  int angle = 600;
   while(angle != -200)
  {
      Dynamixel.moveSpeed(DYMX_ID_DOOR, (500 + (6 * angle / 18)), 1000);
      angle -= 10;
        delay(20);
  }
  delay(100);
}

void front_up() 
{
  int angle = DOWN_ANGLE;
  while(angle != UP_ANGLE)
  {
      Dynamixel.moveSpeed(DYMX_ID_LEFT, (500 + (6 * angle / 18)), 1000);
      Dynamixel.moveSpeed(DYMX_ID_RIGHT, (500 - (6 * angle / 18)), 1000);
      angle += 10;
        delay(20);
  }
  delay(100);
}

void front_down()
{
  int angle = UP_ANGLE;
  while(angle != DOWN_ANGLE)
  {
  	Dynamixel.moveSpeed(DYMX_ID_LEFT, (500 + (6 * angle / 18)), 1000);
  	Dynamixel.moveSpeed(DYMX_ID_RIGHT, (500 - (6 * angle / 18)), 1000);
   angle -= 10;
    delay(20);
  }
    delay(100);

}
