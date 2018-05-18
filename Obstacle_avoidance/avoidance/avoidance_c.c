#include "hcsr04.h"

#define SENSOR_NUMBER 6
#define ENABLE_US     50

// Weights for the braitenberg obstacle avoidance
int w_right[SENSOR_NUMBER] = {1,1,1,1,1,1};
int w_left[SENSOR_NUMBER]  = {1,1,1,1,1,1};

int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M1 Direction Control

// TEST this with int or long
void (char left,char right)          //Move forward
{
  if(left > 0 && right > 0)
  {
    analogWrite (E1,left);      //PWM Speed Control
    digitalWrite(M1,HIGH);    
    analogWrite (E2,right);    
    digitalWrite(M2,HIGH);
  }
  else if(left < 0 && right > 0)
  {
    analogWrite (E1,left);      //PWM Speed Control
    digitalWrite(M1,LOW);    
    analogWrite (E2,right);    
    digitalWrite(M2,HIGH);
  }
  else if(left > 0 && right < 0)
  {
    analogWrite (E1,left);      //PWM Speed Control
    digitalWrite(M1,HIGH);    
    analogWrite (E2,right);    
    digitalWrite(M2,LOW);
  }
  else if(left < 0 && right < 0)
  {
    analogWrite (E1,left);      //PWM Speed Control
    digitalWrite(M1,LOW);    
    analogWrite (E2,right);    
    digitalWrite(M2,LOW);
  }
}

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

HCSR04 ultrasonic_sensors[SENSOR_NUMBER];


void setup() 
{
  // put your setup code here, to run once:
  

  //Setup for the motor driver
  int i;
  for(i=4;i<=7;i++)
  {
    	pinMode(i, OUTPUT);  
  }
  
  Serial.begin(9600);      //Set Baud Rate and start communication
  Serial.println("Run keyboard control");

  //Init the ultrasonic sensors (TRIG_PIN, ECHO_PIN)
  //Faut que je test au niveua electrique un cable plat avec des pins males brancher dedans
  int j = 0;
  for(i = 0; i < SENSOR_NUMBER; i++)
  {
    ultrasonic_sensors[i].init(j+40,j+41);
    j += 2;
  }



  //Init the motors
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);

  pinMode(ENABLE_US,OUPUT);
  digitalWrite(ENABLE_US,LOW);

}

void get_brait_speed(int* speed_right, int* speed_left)
{
  //loop counter
  int i;
  int distance = 0;
  for(i = 0; i < SENSOR_NUMBER; i++)
  {
    distance = ultrasonic_sensors[i].readDisctanceInCm();
    *speed_right = (*speed_right) + *speed_right * (w_right[i]*distance);
    *speed_left  = (*speed_left)  + *speed_left *(w_left[i]*distance);
  }
}