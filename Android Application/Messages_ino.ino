#include<Servo.h>

Servo LServo;
Servo RServo;

void setup()
{
  //This will connect to the correct pins of the Servo Motors
  LServo.attach(6);
  RServo.attach(5);
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  bool Received = false;
  char chat[2];
  
  while(!Received)
  {
    while(Serial.available())
    {
       Serial.readBytes(chat,2);
       Received = true; 
    }
  }
  
  LServo.writeMicroseconds(1500+chat[0]*2);
  RServo.writeMicroseconds(1500-chat[1]*2);
  
}
