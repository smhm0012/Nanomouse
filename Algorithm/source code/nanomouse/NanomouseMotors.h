#include <Arduino.h>
#include <Servo.h>

#define RIGHT 1
#define LEFT -1

class NanomouseMotors {
private:
  Servo leftServo;
  Servo rightServo;

  static const byte power = 100;
public:
  void attach(byte leftMotor, byte rightMotor)
  {
    leftServo.attach(leftMotor); //Arduino Pin 6
    rightServo.attach(rightMotor); //Arduino Pin 5
  }
  void forward()
  {
    //moving forward
    /*leftServo.writeMicroseconds(1700);
     rightServo.writeMicroseconds(1300);*/
    leftServo.writeMicroseconds(1500+power);
    rightServo.writeMicroseconds(1500-power);

  }
  void stop(int time = 200)//Delay for better performence
  {
    //Stop position
    leftServo.writeMicroseconds(1500);
    rightServo.writeMicroseconds(1500);
    delay(time);
  }
  void turnRight()
  {
    /* //moving right
     leftServo.writeMicroseconds(1700);
     rightServo.writeMicroseconds(1700); */
    leftServo.writeMicroseconds(1500+power);
    rightServo.writeMicroseconds(1500+power);
  }
  void turnLeft()
  {
    /* //moving left
     leftServo.writeMicroseconds(1300);
     rightServo.writeMicroseconds(1300); */
    leftServo.writeMicroseconds(1500-power);
    rightServo.writeMicroseconds(1500-power);
  }
  void forwardTime(unsigned int time) //unsigned does not have negative value. Plus larger than int
  {
    forward();
    delay(time);
    stop();
  }
  void backward()
  {
     //moving backward
     leftServo.writeMicroseconds(1300);
     rightServo.writeMicroseconds(1700);
  }
  void turnRightTime(int time)
  {
    turnRight();
    delay(time);
    stop();
  }
  void turnLeftTime(int time)
  {
    turnLeft();
    delay(time);
    stop();
  }
  void turn (int direction, int degree)
  {
    leftServo.writeMicroseconds(1500+power*direction);
    rightServo.writeMicroseconds(1500+power*direction);
    delay(degree*5);
    stop();
  }
  
  void forwardProportional(int error)
  {
    //moving forward Proportionaly
    const byte Kp = 2;
    leftServo.writeMicroseconds(1500+power-Kp*error);
    rightServo.writeMicroseconds(1500-power-Kp*error);

  }
};


