 #include "motor_control.h"
#include "braitenberg.h"
#include "manual_mode.h"
#include "dyn_control.h"

static char* in_buffer;
static int length = 10;

static int speed_right;
static int speed_left;

static int sign = 1;
static int brait_active = 0;

void setup()
{
	//Starts the communication with the raspberry pi (or computer)
	Serial.begin(9600);    
  
  init_motor();

	init_us_sensors();

	init_manual_mode();

	init_dyn();

	speed_right = 0;
	speed_left = 0;
	brait_active = 0;
}

void decode_buffer()
{

}

void send_data()
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
				Serial.println("Manual mode activated");
				manual_mode_main(0);
				break;
			}
			case '1':
			{
				decode_buffer();
				//get speed from raspberry pi here
				break;
			}
		}
	}
	
	//update the speed according to the braitenberg
	get_brait_speed(&speed_left, &speed_right,sign,&brait_active);
	
	//set the speed here
	max_min_speed(&speed_left, &speed_right);

	set_speed(0,0);	
}
