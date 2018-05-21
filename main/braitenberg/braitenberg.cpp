#include "braitenberg.h"

// Weights for the braitenberg obstacle avoidance
static int w_right[SENSOR_NUMBER] = { 1,1,1,1,1,1 };
static int w_left[SENSOR_NUMBER] = { 1,1,1,1,1,1 };
static HCSR04 ultrasonic_sensors[SENSOR_NUMBER];


//declare all HC-SR04 ultrasonic senors
/*

  right_back.init(40,41);
  right_front.init(42,43);

  front_right.init(44,45);
  //front_mid.init(46,47);
  front_left.init(48.49)

  left_front.init(50,51);
  left_back.init(52,53);

 */

void init_us_sensors()
{
	//Init the ultrasonic sensors (TRIG_PIN, ECHO_PIN)
	//Faut que je test au niveua electrique un cable plat avec des pins males brancher dedans
	int i = 0;
	int j = 0;
	for (i = 0; i < SENSOR_NUMBER; i++)
	{
		ultrasonic_sensors[i].init(j + 40, j + 41);
		j += 2;
	}
}

void get_brait_speed(int* speed_right, int* speed_left)
{
	//loop counter
	int i;
	int distance = 0;
	for (i = 0; i < SENSOR_NUMBER; i++)
	{
		distance = ultrasonic_sensors[i].readDisctanceInCm();
		*speed_right = (*speed_right) + (w_right[i] * distance);
		*speed_left = (*speed_left) + (w_left[i] * distance);
	}
}
