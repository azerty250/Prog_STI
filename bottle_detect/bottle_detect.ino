/***************************************************************************************
 *
 * Title:       Analog read IR
 * Description: Read an infrared sensor from an analog input port.
 * If the analog value read is < 512, we turn the LED on, else it's turned off.
 * Connect the sensor output to A1.
 * Connect the sensor power rack ENABLE to 4.
 *
 ***************************************************************************************/
//#include <prismino.h>
#include <Arduino.h>

#define LED 13
#define IR_EN 10

// the analog value is between 0 and 1023 so it needs a 16-bit variable
// can use "unsigned int" or "uint16_t" for short
unsigned int ambient1;
unsigned int ambient2;
unsigned int ir_value_1;
unsigned int ir_value_2;
unsigned int bottle_timeout;

void setup()
{
  // set pin output mode (sources current)
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(IR_EN, OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  bottle_timeout = 0;
}

void loop()
{
  // turn on the emitter and read the IR sensor
  ambient1 = analogRead(A1);
  digitalWrite(IR_EN, HIGH); 
  delay(1);
  ir_value_1 = analogRead(A1);
  digitalWrite(IR_EN, LOW);

  ir_value_1 -= ambient1;
  
  ambient2 = analogRead(A2);
  digitalWrite(11, HIGH); 
  delay(1);
  ir_value_2 = analogRead(A2);
  digitalWrite(11, LOW);

  ir_value_2 -= ambient2;

  /*Serial.print(ir_value_1);
  Serial.print("          ");
  Serial.println(ir_value_2);*/
  delay(5);

  if(((ir_value_1 > 20)||(ir_value_2 < 50)) && (millis()-bottle_timeout>1200))
  {
    bottle_timeout = millis();
    Serial.println("BOTTLE DETECTED");
  }
}

