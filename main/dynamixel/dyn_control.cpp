#include "dyn_control.h"

void init_dyn()
{

	Serial.begin(57600);

	Dynamixel.begin(1000000, 2);  // Initialize the servo at 1Mbps and Pin Control 2


	Dynamixel.setEndless(DYMX_ID_LEFT, OFF);
	Dynamixel.setEndless(DYMX_ID_RIGHT, OFF);

	delay(1000);
}


void front_up() 
{
	Dynamixel.moveSpeed(DYMX_ID_LEFT, (500 + (6 * UP_ANGLE / 18)), 1000);
	Dynamixel.moveSpeed(DYMX_ID_RIGHT, (500 - (6 * UP_ANGLE / 18)), 1000);

	delay(20);
}

void front_down()
{
	Dynamixel.moveSpeed(DYMX_ID_LEFT, (500 + (6 * DOWN_ANGLE / 18)), 1000);
	Dynamixel.moveSpeed(DYMX_ID_RIGHT, (500 - (6 * DOWN_ANGLE / 18)), 1000);

	delay(20);
}