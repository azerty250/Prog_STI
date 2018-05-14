

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

void setup() 
{
  // put your setup code here, to run once:
  
  int i;
  for(i=4;i<=7;i++)
  {
    	pinMode(i, OUTPUT);  
  }
  
  Serial.begin(9600);      //Set Baud Rate and start communication
  Serial.println("Run keyboard control");

  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);

  pinMode(ENABLE_US,OUPUT);
  digitalWrit(ENABLE_US,LOW);

}

void loop()
{
  //loop counter
  int i;



  for(i = 0; i < SENSOR_NUMBER; i++)
  {

  }


}
