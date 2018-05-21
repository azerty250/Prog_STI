#include "motor/motor_control.h"
#include "braitenberg/braitenberg.h" 

static char* in_buffer;
static int length = 10;

void setup()
{
	//Starts the communication with the raspberry pi (or computer)
	Serial.begin(9600);    

	init_motor();

	init_us_sensors();
}

void decode_buffer()
{

}

void loop()
{
	if (Serial.available())
	{
		Serial.readBytes(in_buffer,length);

		//To define robot and attitude and control from the raspberry pi
		switch (in_buffer[0])
		{
			case '0':
			{
				//go to manual mode
				break;
			}
			case '1':
			{
				decode_buffer();
				set_speed(0,0);
				break;
			}
		}
	}
}