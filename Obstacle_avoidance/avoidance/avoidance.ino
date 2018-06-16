#include "hcsr04.h"

#define SENSOR_NUMBER 6
#define ENABLE_US     50

// Weights for the braitenberg obstacle avoidance
int w_right[SENSOR_NUMBER] = {1,1,1,1,1,1};
int w_left[SENSOR_NUMBER]  = {1,1,1,1,1,1};



//declare all HC-SR04 ultrasonic senors here
//HCSR04 front_mid;
/*
HCSR04 front_left;
HCSR04 front_right;

HCSR04 left_front;
HCSR04 left_back;

HCSR04 right_front;
HCSR04 right_back;

  right_back.init(40,41);
  right_front.init(42,43);

  front_right.init(44,45);
  //front_mid.init(46,47);
  front_left.init(48.49)

  left_front.init(50,51);
  left_back.init(52,53);*/

//HCSR04 ultrasonic_sensors[SENSOR_NUMBER];
HCSR04 test_us;

void setup() 
{
  // put your setup code here, to run once:

  test_us.init(11,10);
  
  Serial.begin(9600);      //Set Baud Rate and start communication
}

void loop()
{
  int distance = test_us.readDisctanceInCm();
  Serial.println(distance);
  delay(10);
}
