#include <Arduino.h>

#include <DynamixelSerial.h>

#define DYMX_ID 11
#define SCAN_MAX_ANGLE 900
#define SCAN_SPEED 10
#define SCAN_SIZE  20
#define DYN2 5

int i;

int scan_angle;
int scan_direction;
int mode;


#define E1  5     //M1 Speed Control
#define E2  6     //M2 Speed Control
#define M1  4     //M1 Direction Control
#define M2  7    //M1 Direction Control

#define PIN_RX_1 28 // pin for input num * from receiver
#define PIN_RX_2 29 // (*=1,2,3,4)
#define PIN_RX_3 30
#define PIN_RX_4 31

#define RX_1 0 // index for memory for input num * from receiver
#define RX_2 1 // (*=1,2,3,4)
#define RX_3 2
#define RX_4 3 
 
volatile unsigned long time_high[4] = {0,0,0,0}; // value of time in ms during wich PIN_RX_* is HIGH (*=1,2,3,4)

#define ledPin 13


void setup() {

  Serial.begin(9600);

  Dynamixel.begin(1000000,2);  // Initialize the servo at 1Mbps and Pin Control 2

  
  Dynamixel.setEndless(DYMX_ID, OFF);
   Dynamixel.setEndless(5, OFF);

  //scan_angle=0;
  //scan_direction=SCAN_SPEED;


//Motors
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

    scan_angle = 100;

        int angle = -200;
  while(angle != 800)
  {
      Dynamixel.moveSpeed(7, (500 + (6 * angle / 18)), 1000);
      angle += 10;
        delay(20);
  }

  analogWrite(10,200);
  delay(100);
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
    analogWrite (E1,b);      //PWM Speed Control
      digitalWrite(M1,HIGH);    

  }
  else if(left < 0 && right < 0)
  {
    a = (char) -left;
    b = (char) -right;
    analogWrite (E1,b);      //PWM Speed Control
      digitalWrite(M1,LOW);    
      analogWrite (E2,a);    
      digitalWrite(M2,LOW);
  }
  else if(left < 0 && right > 0)
  {
    a = (char) -left;
    b = (char) right;
    analogWrite (E1,b);      //PWM Speed Control
      digitalWrite(M1,HIGH);    
      analogWrite (E2,a);    
      digitalWrite(M2,LOW);
  }
  else if(left > 0 && right < 0)
  {
    a = (char) left;
    b = (char) -right;
    analogWrite (E1,b);      //PWM Speed Control
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


void shake()
{
  set_speed(180, 180);
  delay(400);
  set_speed(-180,-180);
  delay(400);
   set_speed(180, 180);
  delay(400);
  set_speed(-180,-180);
  delay(400);
  set_speed(180,180);
  delay(300);
  set_speed(0, 0);
}


void up()
{
    while(scan_angle< SCAN_MAX_ANGLE) 
    {
      scan_angle+=10;
  
      Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
      Dynamixel.moveSpeed(DYN2,(500-(6*scan_angle/18)),1000); 
        //Serial.print(mode);
  //Serial.print(" ");
  //Serial.println(scan_angle);
      delay(15);
    }
}

void down()
{
    while(scan_angle >0)
    {
      scan_angle-=10;
  
      Dynamixel.moveSpeed(DYMX_ID,(500+(6*scan_angle/18)),1000);   
      Dynamixel.moveSpeed(DYN2,(500-(6*scan_angle/18)),1000); 
        //Serial.print(mode);
  //Serial.print(" ");
  //Serial.println(scan_angle);
      delay(15);
    }
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
      set_speed(150,180);
  }
  else if(time_high[1] > 1950 && time_high[1] < 2000)
  {
      set_speed(-150,-180);
  }  
  else if(time_high[0] > 1000 && time_high[0] < 1100)
  {
      set_speed(-150,150);
  }
  else if(time_high[0] > 1950 && time_high[0] < 2000)
  {
      set_speed(150,-150);
  }
  else if(time_high[2] > 1900 && time_high[2] < 2000)
  {
     scan_angle=SCAN_MAX_ANGLE;
     down();
  }
  else if(time_high[2] > 1000 && time_high[2] < 1100)
  {
    scan_angle = 0;
    up();
  }  
  else if(time_high[3] > 1920 && time_high[3] < 2000)
  {
    shake();
  }
  else
  {     
    set_speed(0,0);
  }

  delay(100);
 
}
