#define mcu 4 //mcu sginal pin
#define pump 3 //pump pin

void setup() {
  pinMode(mcu,INPUT);//setting GPIO4 as INPUT
  pinMode(pump,OUTPUT);//setting GPIO4 as OUTPUT
  Serial.begin(9600);

}

void loop() {
  digitalWrite(pump,LOW);//disabel pump
  int x=digitalRead(mcu);//read pin
  Serial.println(x);
  if(x==HIGH)
  {
    digitalWrite(pump,HIGH);//power the pump if HIGH
  }
  delay(100);//keep pump on for 100 ms
}
