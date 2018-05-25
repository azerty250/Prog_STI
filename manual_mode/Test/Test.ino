#include <Arduino.h>

#define E1  5     //M1 Speed Control
#define E2  6     //M2 Speed Control
#define M1  4     //M1 Direction Control
#define M2  7    //M1 Direction Control

#define PIN_RX_1 32 // pin for input num * from receiver
#define PIN_RX_2 30 // (*=1,2,3,4)
#define PIN_RX_3 28
#define PIN_RX_4 26

#define RX_1 0 // index for memory for input num * from receiver
#define RX_2 1 // (*=1,2,3,4)
#define RX_3 2
#define RX_4 3 
 
volatile unsigned long time_high[4] = {0,0,0,0}; // value of time in ms during wich PIN_RX_* is HIGH (*=1,2,3,4)

#define ledPin 13


void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  pinMode(PIN_RX_1, INPUT);
  pinMode(PIN_RX_2, INPUT); 
  pinMode(PIN_RX_3, INPUT); 
  pinMode(PIN_RX_4, INPUT);

  int i;
  for(i=4;i<=7;i++)
  {
     pinMode(i, OUTPUT);  
  }
  
  Serial.println("Start manual mode");
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
 
void loop() 
{ 
    time_high[0] = pulseIn(PIN_RX_1, HIGH); //left and right (right joystick)
    time_high[1] = pulseIn(PIN_RX_2, HIGH); //front and back (right joystick)
    time_high[2] = pulseIn(PIN_RX_3, HIGH); // front and back (left joystick)
    time_high[3] = pulseIn(PIN_RX_4, HIGH); // left and right (left joystick)

  Serial.print(time_high[0]);  
  Serial.print("  ");
  Serial.print(time_high[1]);
  Serial.print("  ");
  Serial.print(time_high[2]);  
  Serial.print("  ");
  Serial.println(time_high[3]);

  if(time_high[1] > 1000 && time_high[1] < 1050)
  {
      set_speed(150,150);
  }
  else if(time_high[1] > 1950 && time_high[1] < 2000)
  {
      set_speed(-150,-150);
  }  
  else if(time_high[0] > 1050 && time_high[0] < 1100)
  {
      set_speed(150,-150);
  }
  else if(time_high[0] > 1950 && time_high[0] < 2000)
  {
      set_speed(-150,150);
  }
  else
  {     
    stop();
  }
 
}
