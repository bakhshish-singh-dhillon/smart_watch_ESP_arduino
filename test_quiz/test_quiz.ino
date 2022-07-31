int flag;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  flag=0;
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(16,INPUT);

  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(28,OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  for(int i=23;i<29;i++)
  {
    digitalWrite(i,!(digitalRead(i)));
    delay(1000);
  }

}
