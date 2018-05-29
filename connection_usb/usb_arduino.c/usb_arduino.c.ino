int compteur = 0;
String message;

void setup(){
Serial.begin(9600);
}


void loop() {
message = "-";
if (Serial.available()) {
message = Serial.readString();
}
Serial.print("MSG # ");
Serial.print(compteur);
Serial.print(" read : ");
Serial.println(message);

compteur++;
delay(1000);
}

