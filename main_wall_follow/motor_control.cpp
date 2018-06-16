#include "motor_control.h"

//Use pins with interruprs
static Encoder enc_right(3, 2);
static Encoder enc_left(21, 20);

static long new_right = 0;
static long new_left = 0;

static long positionLeft = 0;
static long positionRight = 0;

void init_motor()
{
	int i;
	for (i = 4; i <= 7; i++)
	{
		pinMode(i, OUTPUT);
	}

	digitalWrite(E1, LOW);
	digitalWrite(E2, LOW);
	pinMode(2, INPUT);
	pinMode(3, INPUT);
}

void max_min_speed(int* speed_left, int* speed_right)
{
	//Maximum is normally 255 but the cap here is at 200 to avoid going over 12V
	//on the motors
	if (*speed_left > 200)
	{
		*speed_left = 200;
	}
	else if (*speed_left < -200)
	{
		*speed_left = -200;
	}


	if (*speed_right > 200)
	{
		*speed_right = 200;
	}
	else if (*speed_right < -200)
	{
		*speed_right = -200;
	}
}

void shake()
{
	set_speed(170, -170);
	delay(200);
	set_speed(-170, 170);
	delay(300);
	set_speed(170, -170);
	delay(300);
	set_speed(-170, 170);
	delay(300);
	set_speed(170, -170);
	delay(300);
	set_speed(-170, 170);
	delay(300);
	set_speed(0, 0);
}

void set_speed(int left, int right)          //Move forward
{
	char a,b;
	if(left > 0 && right > 0)
	{
		a = (char) left;
		b = (char) right;
		analogWrite (E2,a);    
  		digitalWrite(M2,HIGH);
		analogWrite (E1,b);      //Forward control
  		digitalWrite(M1,HIGH);    

	}
	else if(left < 0 && right < 0)
	{
		a = (char) -left;
		b = (char) -right;
		analogWrite (E1,b);      //Backward control
  		digitalWrite(M1,LOW);    
  		analogWrite (E2,a);    
  		digitalWrite(M2,LOW);
	}
	else if(left < 0 && right > 0)
	{
		a = (char) -left;
		b = (char) right;
		analogWrite (E1,b);      //Turn left control
  		digitalWrite(M1,HIGH);    
  		analogWrite (E2,a);    
  		digitalWrite(M2,LOW);
	}
	else if(left > 0 && right < 0)
	{
		a = (char) left;
		b = (char) -right;
		analogWrite (E1,b);      //Turn right control
  		digitalWrite(M1,LOW);    
  		analogWrite (E2,a);    
  		digitalWrite(M2,HIGH);
	}
	else
	{
		digitalWrite(E1, 0);
		digitalWrite(M1, LOW);
		digitalWrite(E2, 0);
		digitalWrite(M2, LOW);
	}
}

float get_diff_enc_right()
{
	return enc_right.read();
}

void reset_enc_right()
{
	enc_right.write(0);
}

float get_diff_enc_left()
{
	return enc_left.read();
}

void reset_enc_left()
{
	enc_left.write(0);
}

/*
new_right = enc_right.read();
new_left = enc_left.read();

if (new_left != positionLeft || new_right != positionRight)
{
Serial.print("Left = ");
Serial.print(new_left);
Serial.print(", Right = ");
Serial.print(new_right);
Serial.println();
positionLeft = new_left;
positionRight = new_right;
}*/

#define ERROR_RES  5
#define COR_FACTOR 10
#define MAX_SPEED  200

void motor_control_main(int goal_x, int goal_y, int pos_x, int pos_y, int* speed_left, int* speed_right)
{
	int error_x = goal_x - pos_x;
	int error_y = goal_y - pos_y;

	//regule l'orientation du robot pour aller dans la bonne direction puis bouge en ligne droite
	//si obs avoid active il faut reevaluer apres pour reprendr la bonne direction
	//is goal reached fonction avant de passer au waypoint et reevaluer la direction

}
