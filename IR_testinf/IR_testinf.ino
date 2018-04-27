int IR_value;
int temp1, temp2;

void setup() {
  // put your setup code here, to run once:
  pinMode(A2, INPUT);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  temp1 = analogRead(A2);
  digitalWrite(13,LOW);
  temp2 = analogRead(A2);
  
  IR_value = temp1 - temp2;
  
  Serial.println(IR_value);

}
