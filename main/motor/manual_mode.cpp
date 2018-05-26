#include "manual_mode.h"

static long int time_high[4] = { 0,0,0,0 };

void init_manual_mode()
{
	pinMode(PIN_RX_1, INPUT);
	pinMode(PIN_RX_2, INPUT);
	pinMode(PIN_RX_3, INPUT);
	pinMode(PIN_RX_4, INPUT);
}


void manual_mode_main(int verbose)
{
	time_high[0] = pulseIn(PIN_RX_1, HIGH); //left and right (right joystick)
	time_high[1] = pulseIn(PIN_RX_2, HIGH); //front and back (right joystick)
	time_high[2] = pulseIn(PIN_RX_3, HIGH); // front and back (left joystick)
	time_high[3] = pulseIn(PIN_RX_4, HIGH); // left and right (left joystick)

	if (verbose)
	{
		Serial.print(time_high[0]);
		Serial.print("  ");
		Serial.print(time_high[1]);
		Serial.print("  ");
		Serial.print(time_high[2]);
		Serial.print("  ");
		Serial.println(time_high[3]);
	}

	if (time_high[1] > 1000 && time_high[1] < 1050)
	{
		set_speed(150, 150); 
	}
	else if (time_high[1] > 1950 && time_high[1] < 2000)
	{
		set_speed(-150, -150);
	}
	else if (time_high[0] > 1050 && time_high[0] < 1100)
	{
		set_speed(150, -150);
	}
	else if (time_high[0] > 1950 && time_high[0] < 2000)
	{
		set_speed(-150, 150);
	}
	else
	{
		stop();
	}

}