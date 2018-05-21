#include "Encoder.h"

int counter = 0;

void current_sense()                  // current sense and diagnosis
{
  int val1 = digitalRead(2);
  int val2 = digitalRead(3);
  if (val1 == HIGH || val2 == HIGH) {
    counter++;
    if (counter == 3) {
      counter = 0;
      Serial.println("Warning");
    }
  }
}

//Use pins with interruprs
Encoder enc_right(18, 19);
Encoder enc_left(20, 21);

long positionLeft  = 0;
long positionRight = 0;

void setup(void)
{
  Serial.begin(9600);
}

long new_right = 0;
long new_left = 0;

void loop(void)
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
