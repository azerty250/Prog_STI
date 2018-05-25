//Pin Change Interrupts
#include "PinChangeInt.h"

//#define NO_PORTB_PINCHANGES //PinChangeInt setup
#define NO_PORTC_PINCHANGES //only port D pinchanges

#define PIN_RX_1 32 // pin for input num * from receiver
#define PIN_RX_2 30 // (*=1,2,3,4)
#define PIN_RX_3 28
#define PIN_RX_4 26

#define RX_1 0 // index for memory for input num * from receiver
#define RX_2 1 // (*=1,2,3,4)
#define RX_3 2
#define RX_4 3 
 
volatile unsigned long time_high[4] = {0,0,0,0}; // value of time in ms during wich PIN_RX_* is HIGH (*=1,2,3,4)
volatile unsigned long prev_time[4] = {0,0,0,0}; // value of time in ms when PIN_RX_* start to be HIGH (*=1,2,3,4)
uint8_t latest_interrupted_pin;
String to_print;
unsigned long t0=0, t=0;
int count=0;

#define ledPin 13

// interrupt routine for rising case of receiver 
void rising()
{
  latest_interrupted_pin=PCintPort::arduinoPin; // get the latest interrupted pin
  switch(latest_interrupted_pin) // switch for that pin to know if it's an PIN_RX_*
  {
    case PIN_RX_1:
     PCintPort::attachInterrupt(PIN_RX_1, &falling, FALLING); // attach a new interrupt for falling case
     prev_time[RX_1] = micros(); // save the time of rising
     count++;
     break;
    case PIN_RX_2:
     PCintPort::attachInterrupt(PIN_RX_2, &falling, FALLING);
     prev_time[RX_2] = micros();
     break;
    case PIN_RX_3:
     PCintPort::attachInterrupt(PIN_RX_3, &falling, FALLING);
     prev_time[RX_3] = micros();
     break;
    case PIN_RX_4:
     PCintPort::attachInterrupt(PIN_RX_4, &falling, FALLING);
     prev_time[RX_4] = micros();
     break;
    default:
     break;
  }
}
 
void falling() {
  latest_interrupted_pin=PCintPort::arduinoPin; // get the latest interrupted pin
  switch(latest_interrupted_pin) // switch for that pin to know if it's an PIN_RX_*
  {
    case PIN_RX_1:
     PCintPort::attachInterrupt(PIN_RX_1, &rising, RISING); // attach a new interrupt for rising case
     time_high[RX_1] = micros()-prev_time[RX_1]; // save the time of high
     break;
    case PIN_RX_2:
     PCintPort::attachInterrupt(PIN_RX_2, &rising, RISING);
     time_high[RX_2] = micros()-prev_time[RX_2];
     break;
    case PIN_RX_3:
     PCintPort::attachInterrupt(PIN_RX_3, &rising, RISING);
     time_high[RX_3] = micros()-prev_time[RX_3];
     break;
    case PIN_RX_4:
     PCintPort::attachInterrupt(PIN_RX_4, &rising, RISING);
     time_high[RX_4] = micros()-prev_time[RX_4];
     break;
    default:
     break;
  }
}

ISR(TIMER1_COMPA_vect)// timer compare interrupt service routine
{
  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin
  // display last value of controler
  //Serial.print("Time per channel : ");
  to_print="ch1 : ";
  to_print+=time_high[RX_1];
  to_print+=" ch2 : ";
  to_print+=time_high[RX_2];
  to_print+=" ch3 : ";
  to_print+=time_high[RX_3];
  to_print+=" ch4 : ";
  to_print+=time_high[RX_4];
  //Serial.println(to_print);

  //Serial.print("count: ");
  //Serial.println(count);
  count=0;
}

void setup() {

  Serial.begin(9600);
    noInterrupts();           // disable all interrupts

  Serial.println("b;albkev");
  interrupts();
  pinMode(ledPin, OUTPUT);

  pinMode(PIN_RX_1, INPUT); digitalWrite(PIN_RX_1, HIGH);
  pinMode(PIN_RX_2, INPUT); digitalWrite(PIN_RX_2, HIGH);
  pinMode(PIN_RX_3, INPUT); digitalWrite(PIN_RX_3, HIGH);
  pinMode(PIN_RX_4, INPUT); digitalWrite(PIN_RX_4, HIGH);
  
  noInterrupts();           // disable all interrupts
  PCintPort::attachInterrupt(PIN_RX_1, &rising, RISING);
  PCintPort::attachInterrupt(PIN_RX_2, &rising, RISING);
  PCintPort::attachInterrupt(PIN_RX_3, &rising, RISING);
  PCintPort::attachInterrupt(PIN_RX_4, &rising, RISING);
  interrupts();             // enable all interrupts
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 31250;            // compare match register 16MHz/256/2Hz=31250
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
  
}
 
void loop() 
{
  Serial.print(time_high[0]);  
  Serial.print(' ');
  Serial.print(time_high[1]);
  Serial.print(' ');
  Serial.print(time_high[2]);  
  Serial.print(' ');
  Serial.println(time_high[3]); }
