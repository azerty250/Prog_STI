#include "Encoder.h"

#define E1  5     //M1 Speed Control
#define E2  6     //M2 Speed Control
#define M1  4     //M1 Direction Control
#define M2  7    //M1 Direction Control

//Use pins with interruprs
static Encoder enc_left(2, 3);
static Encoder enc_right(21, 20);

long new_right = 0;
long new_left = 0;

long positionLeft = 0;
long positionRight = 0;

long int count = 0;


void setup()
{
   int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);  
  Serial.begin(9600);      //Set Baud Rate
  Serial.println("Run keyboard control");
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
}

void set_speed(char left, char right)          //Move forward
{
  if (left > 0 && right > 0)
  {
    analogWrite(E1, left);      //PWM Speed Control
    digitalWrite(M1, HIGH);
    analogWrite(E2, right);
    digitalWrite(M2, HIGH);
  }
  else if (left < 0 && right > 0)
  {
    analogWrite(E1, left);      //PWM Speed Control
    digitalWrite(M1, LOW);
    analogWrite(E2, right);
    digitalWrite(M2, HIGH);
  }
  else if (left > 0 && right < 0)
  {
    analogWrite(E1, left);      //PWM Speed Control
    digitalWrite(M1, HIGH);
    analogWrite(E2, right);
    digitalWrite(M2, LOW);
  }
  else if (left < 0 && right < 0)
  {
    analogWrite(E1, left);      //PWM Speed Control
    digitalWrite(M1, LOW);
    analogWrite(E2, right);
    digitalWrite(M2, LOW);
  }
}

void read_enc()
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

void advance(char a,char b)          //Move forward
{
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
} 

void stop(void)                    //Stop
{
  digitalWrite(E1,0); 
  digitalWrite(M1,LOW);    
  digitalWrite(E2,0);   
  digitalWrite(M2,LOW);    
} 

void loop(void)
{
  if(Serial.available())
  {
      char val = Serial.read();
      if(val== 'w')
      {
        enc_right.write(0);
        enc_left.write(0);
           set_speed(150,150);
           read_enc();
          delay(1000);
          read_enc();
          stop();
      }
  }
}



















