#include "motor_control.h"

//Use pins with interruprs
static Encoder enc_right(18, 19);
static Encoder enc_left(20, 21);

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

void set_speed(char left, char right)          //Move forward
{
	if (left > 0 && right > 0)
	{
		analogWrite(E1, left);      //PWM Speed Control
		digitalWrite(M1, LOW);
		analogWrite(E2, right);
		digitalWrite(M2, LOW);
	}
	else if (left < 0 && right > 0)
	{
		analogWrite(E1, left);      //PWM Speed Control
		digitalWrite(M1, HIGH);
		analogWrite(E2, right);
		digitalWrite(M2, LOW);
	}
	else if (left > 0 && right < 0)
	{
		analogWrite(E1, left);      //PWM Speed Control
		digitalWrite(M1, LOW);
		analogWrite(E2, right);
		digitalWrite(M2, HIGH);
	}
	else if (left < 0 && right < 0)
	{
		analogWrite(E1, left);      //PWM Speed Control
		digitalWrite(M1, HIGH);
		analogWrite(E2, right);
		digitalWrite(M2, HIGH);
	}
}

long get_diff_enc_right()
{
	return enc_right.read();
}

void reset_enc_right()
{
	enc_right.write(0);
}

long get_diff_enc_left()
{
	return enc_left.read();
}

void reset_enc_left()
{
	enc_left.write(0);
}

void motor_control_main(void)
{
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
	}
}
