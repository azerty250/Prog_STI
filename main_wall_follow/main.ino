#include <HCSR04.h>

#include "motor_control.h"
//#include "braitenberg.h"
#include "manual_mode.h"
#include "dyn_control.h"

#define LEFT 0
#define RIGHT 1
#define ENCODER_SCALE_FACTOR 14
#define TRACK 25

static char *in_buffer;
static int length = 10;

static int speed_right;
static int speed_left;

static float pos_x, pos_y, theta;

static HCSR04 left_us_0(39, 41);
static HCSR04 left_us_1(35, 37);
static HCSR04 right_us_0(47, 49);
static HCSR04 right_us_1(51, 53);

static HCSR04 side_right(50,48);
static HCSR04 side_left(44,40);

static int wall_side;

void setup()
{
  //Starts the communication with the raspberry pi (or computer)
  Serial.begin(57600);

  init_motor();
  
  init_manual_mode();

  init_dyn();

  speed_right = 0;
  speed_left = 0;

  wall_side = 0;
  pos_x = 0;
  pos_y = 0;
  theta = 0;

  pos_x = 0;
  pos_y = 0;

  reset_enc_left();
  reset_enc_right();
  speed_left = 0;
  speed_right = 0;
}

void wall_follow(int* speed_left, int* speed_right)
{
	int i = 0;
  delay(10);
  int distance = side_left.dist();
  delay(10);
	if(wall_side == LEFT)
	{
		if(distance <= 18)
		{
      *speed_left += 50;
      *speed_right -= 20;
		}
		else if(distance >= 25)
		{
			*speed_right += 50;
		}
	}
	else if(wall_side == RIGHT)
	{			
    if(distance <= 18)
    {
      *speed_right += 50;
    }
    else if(distance >= 25)
    {
      *speed_left += 60;
    }											
	}
}

void avoidance()
{
  if(left_us_0.dist() < 35 && right_us_1.dist() < 35)
  {
    //go left or right depending on position and orientation on map
    if(wall_side == LEFT)
    {
      front_up();
      set_speed(120,-120);
      delay(600);
      set_speed(110,135);
      delay(1600);
      set_speed(-120,120);
      delay(520);
      set_speed(110,130);
      delay(200);
      set_speed(0,0);
      front_down();
    }
    else if(wall_side == RIGHT)
    {
      front_up();
      set_speed(-120,120);
      delay(600);
      set_speed(110,135);
      delay(1600);
      set_speed(120,-120);
      delay(520);
      set_speed(110,130);
      delay(200);
      set_speed(0,0);
      front_down();     
    }
  }
  else if(left_us_1.dist() < 50 || right_us_1.dist() < 50)
  {
    //goes right
    front_up();
    set_speed(120,-120);
    delay(600);
    set_speed(110,135);
    delay(1600);
    set_speed(-120,120);
    delay(520);
    set_speed(110,130);
    delay(200);
    set_speed(0,0);
    front_down();
  }
  else if(right_us_0.dist() < 50 || left_us_0.dist() < 50)
  {
    //goes left
    front_up();
    set_speed(-120,120);
    delay(600);
    set_speed(110,135);
    delay(1600);
    set_speed(120,-120);
    delay(520);
    set_speed(110,130);
    delay(200);
    set_speed(0,0);
    front_down();
  }
}
char input_command;    // string to hold input
int front_pos;

//automatically reads the comm port when there is something available
void serialEvent()
{
  read_com();
}

void read_com()
{
    input_command = Serial.read();  
    
    if(input_command == 'g')
    {
      //go left
      speed_left = -120;
      speed_right = 120;
      if(side_left.dist() < 18)
      {
        speed_left = -110;
        speed_right = -110;
      }
    }
    else if(input_command == 'd')
    {
      //go right
      speed_left = 120;
      speed_right = -120;
      if(side_right.dist() < 18)
      {
        speed_left = -110;
        speed_right = -110;
      }
    }
    else if(input_command == 'a')
    {
      //go straight
      speed_left = 90;
      speed_right = 120;
    }
    else if(input_command == 's')
    {
      //stop
      speed_left = 0;
      speed_right = 0;
    }
    else if(input_command == '0')
    {
      //lift front assembly
      front_up();
    }
    else if(input_command == '1')
    {
      //puts down front assembly
      front_down();
    }
    else if(input_command == '2')
    {
      //left side of arena
      wall_side == LEFT;
    }
    else if(input_command == '3')
    {
      //right side of arena
      wall_side == RIGHT;
    }
    else if(input_command == 'e')
    {
      //deposit bottles
      open_gate();
      shake();
      close_gate();
    }
}

void loop()
{
  //update the speed according to the braitenberg
   avoidance();

  //get sure we dont get over 200/200
   max_min_speed(&speed_left, &speed_right);

  set_speed(speed_left,speed_right);
  delay(50);
}
