//#define DEBUG //Comment in to enable debugging codes.

#include <Servo.h>
#include "NanomouseMotors.h"
#include "NanomouseSensors.h"
#include "NanoMouseMaze.h"

//byte to save space. int space not needed
const byte ledPin = 13;
const byte buttonPin = 9;

int targetFront;
int thresholdFront;
int targetSide;
int thresholdSide;

NanomouseMotors motors;

//LeftEmitter, LeftDetector, frontEmitter...
////NanomouseSensors<4, A7, 3, A6, 2, A5> sensors;

NanoMouseMaze<5,5> maze; //Total Rows and Columns. 5 = 0-4

void setup()
{
  // put your setup code here, to run once:
  //pinMode(ledPin,OUTPUT);
  pinMode (buttonPin, INPUT_PULLUP);

  //Arduino left motor Pin 6, and right pin 5
  motors.attach(6,5);

  sensors.configure(); //Trun on infrared emitters

  //Start mouse in certain cell, facing certain position
  maze.mouseRow = 4;
  maze.mouseColumn = 0;
  maze.mouseHeading = NORTH;
  //The finishing point
  maze.targetRow = 3;
  maze.targetColumn = 4;

  Serial.begin(9600);

  //While 'high' do nothing, because switch is inverted. Low will turn it on.
  while(digitalRead(buttonPin))
  {
  }

  delay(500); //Time to take finger off the start button

  calibrate();

  do //Do while not at end of maze
  {
    scanWalls();
    maze.solve();
    
    
    //Stop until botton pressed (debugging)
    #ifdef DEBUG
    maze.print();
    while(digitalRead(buttonPin)){}
    delay(500);
    #endif
    //End debugging
    
    turnTowardBestNeighbor();
    forwardWhiskers();
  }
  while(maze.values[maze.mouseRow][maze.mouseColumn] != 0);
  
  #ifdef DEBUG
  scanWalls();
  maze.solve();
  maze.print();
  #endif
}

void loop()
{
  // put your main code here, to run repeatedly:

  ///avoid(state());
}

void forwardWhiskers()
{
  unsigned long startingTime = millis();
  int oneCell = 1750; //Milliseconds required to travel from one cell's center to another's center (adjust accordingly).

  while(sensors.front < targetFront && millis() - startingTime < oneCell)
  {
    sensors.sense();

    if(sensors.left > thresholdSide && sensors.right > thresholdSide)
      motors.forwardProportional(sensors.right-sensors.left);
    else if(sensors.right > thresholdSide)
      motors.forwardProportional(sensors.right-targetSide);
    else if(sensors.left > thresholdSide)
      motors.forwardProportional(targetSide - sensors.left);
    else
      motors.forward();

  }

  motors.stop();

  maze.mouseRow += neighboringCells[maze.mouseHeading][0];
  maze.mouseColumn += neighboringCells[maze.mouseHeading][0]; //NEWLY ADDED, NOT TESTED!

}

void calibrate()
{
  sensors.initialize();
  targetSide = (sensors.left + sensors.right)/2;

  motors.turn(RIGHT,90);
  sensors.initialize();
  targetFront = sensors.front;
  thresholdSide = (targetSide + sensors.left)/2; //Original
  //thresholdSide = (targetSide + 2*sensors.left)/3;
  /*
    If your robot does not appear to detect walls consistently,
   you could replace the average between the target and
   ambient values used in the previous lecture with values
   that are weighted towards the ambient values.
   For example, if the side sensors miss walls on occasion,
   try calculating the threshold value for the side sensors
   as follows:
   thresholdSide = (targetSide + 2*sensors.left)/3;
   */

  motors.turn(LEFT,90);
  sensors.initialize();

  thresholdFront = (targetFront + sensors.front)/2;
}

void scanWalls()
{
  if(sensors.front > thresholdFront)
  {
    maze.addWalls(maze.mouseHeading);
  }
  if(sensors.right > thresholdSide)
  {
    maze.addWalls((maze.mouseHeading + 1) % 4);
  }
  if(sensors.left > thresholdSide)
  {
    maze.addWalls((maze.mouseHeading + 3) % 4);
  }
}

void turnTowardBestNeighbor()
{
  byte desiredHeading = maze.findBestNeighbor();
  int difference = maze.mouseHeading - desiredHeading;

  if(difference == 1 || difference == -3)//left
  {
    motors.turn(LEFT,90);
  }
  else if(difference == -1 || difference == 3)//right
  {
    motors.turn(RIGHT,90);
  }
  else if(difference == -2 || difference == 2)//180 turn
  {
    motors.turn(LEFT,180);
  }

  maze.mouseHeading = desiredHeading;
}

byte state()
{
  //int threshold = 50; //Test first
  byte event = 0;

  sensors.sense();

  if (sensors.front > thresholdFront)
  {
    event += 1;
  }
  if (sensors.left > thresholdSide)
  {
    event += 2;
  }
  if (sensors.right > thresholdSide)
  {
    event += 4;
  }

  return event;
}

void avoid(byte event)
{
  switch(event)
  {
  case 1: //Front sensor triggered
    //For Demo
    motors.backward();
    delay(1000);
    //End-For Demo
    if(random(2))
    {
      motors.turn(LEFT, random(90,181));
    }
    else
    {
      motors.turn(RIGHT, 90);
    }
    sensors.initialize();
    break;
  case 2: //Left Sensor
    motors.turn(RIGHT, 45);
    sensors.initialize();
    break;
  case 4: //Right sensor
    motors.turn(LEFT, 45);
    sensors.initialize();
    break;
  case 7: //All sensors
    motors.turn(RIGHT, 180);
    sensors.initialize();
    break;
  default:
    motors.forward();
    break;
  }
}

void navigateLabyrinth(byte event)
{
  switch(event)
  {
  case 0:
  case 1:
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
    break;
  case 2:
    forwardWhiskers();
    break;
  case 3:
    motors.turn(RIGHT, 90);
    sensors.initialize();
    break;
  case 4:
    forwardWhiskers();
    break;
  case  5:
    motors.turn(LEFT, 90);
    sensors.initialize();
    break;
  case 6:
    forwardWhiskers();
    break;
  case 7:
    digitalWrite(ledPin, HIGH);
    break;
  }
}













