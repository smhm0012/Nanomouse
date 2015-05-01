#define DEBUG ///Comment in to enable debugging codes.
#define RUN ///Comment out when debugging
#include <Arduino.h> ///Added by Guillermo the Arduino header
#include <Servo.h>
#include <EEPROM.h> ///Added by Guillermo the EEPROM header
#include <string.h> ///Added by Guillermo the string header
#include "NanomouseMotors.h"
#include "NanomouseSensors.h"
#include "NanoMouseMaze.h"
#include "DB.h"     ///Added by Guillermo the DB header

/************************************/
/* By: Guillermo Fabian
/* This is my part for the 
/* Database structure
/*************************************/
DB db;             ///Declare db 

#define MY_TBL 1    ///declare MY_TBL to 1

struct MyRec {      
  byte row;         ///Declare byte row
  byte column;      ///Decalre bye column 
  byte heading;     ///Declare heading 
  int onecell;      ///Declaring onecell
  byte stepno;      ///Declaring stepno
} rec;  

/******************************/
  
///byte to save space. int space not needed
const byte ledPin = 13;
const byte buttonPin = 9;

int targetFront;
int thresholdFront;
int targetSide;
int thresholdSide;

NanomouseMotors motors;

///LeftEmitter, LeftDetector, frontEmitter...
///NanomouseSensors<4, A7, 3, A6, 2, A5> sensors;

NanoMouseMaze<3,4> maze; //Total Rows and Columns. 5 = 0-4

void setup()
{
  /// put your setup code here, to run once:
  ///pinMode(ledPin,OUTPUT);
  pinMode (buttonPin, INPUT_PULLUP);

  ///Arduino left motor Pin 6, and right pin 5
  motors.attach(6,5);

  sensors.configure(); //Trun on infrared emitters

  ///Start mouse in certain cell, facing certain position
  maze.mouseRow = 2;
  maze.mouseColumn = 0;
  maze.mouseHeading = NORTH;
  ///The finishing point
  maze.targetRow = 1;
  maze.targetColumn = 2;
  
/******************************************************/
/* By: Guillermo Fabian
/* Creating the Table and storing the maze positon inside
/* the EEPROM
/******************************************************/
  Serial.begin(9600);
  Serial.println("NanoMouse Database in EEPROM");
  Serial.println();
  
  Serial.print("Creating Table...");
  db.create(MY_TBL,sizeof(MyRec));
  db.open(MY_TBL);
  Serial.println("DONE");
  
  Serial.print("Creating records...");
  
  Serial.print("Writing Mouse Rows: "); Serial.println(rec.row);
  Serial.print("Writing Mouse Columns: "); Serial.println(rec.column);
  Serial.print("Writing Mouse Heading: "); Serial.println(rec.heading);  
  db.append(DB_REC rec);
  Serial.println("DONE");
  
  Serial.println();
  Serial.println("Reading records from EEPROM...");
  Serial.println();
  selectAll();
  
  /**********************************************************************/
  
  ///While 'high' do nothing, because switch is inverted. Low will turn it on.
  while(digitalRead(buttonPin))
  {
  }

  delay(500); ///Time to take finger off the start button

  #ifdef RUN
  calibrate();
  #endif

  do ///Do while not at end of maze
  {
    scanWalls();
    maze.solve();
    
    
    ///Stop until botton pressed (debugging)
    #ifdef DEBUG
    maze.print();
    while(digitalRead(buttonPin)){}
    delay(500);
    #endif
    ///End debugging
    
    #ifdef RUN
    turnTowardBestNeighbor();
    forwardWhiskers();
    #endif
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
  /// put your main code here, to run repeatedly:

  ///avoid(state());
}

/************************************************
/* By: Guillermo Fabian
/* Description: Read the table from the EEPROM, 
/* display it out to the Serial Monitor.
/************************************************/
void selectAll()
{
  if (db.nRecs()) Serial.println("-----");
  for (int i = 1; i <= db.nRecs(); i++)
  {
    db.read(i, DB_REC rec);
    Serial.print("Recnum "); Serial.println(i); 
    Serial.print("Column "); Serial.println(rec.column); 
    Serial.print("Row "); Serial.println(rec.row);
    Serial.print("Heading "); Serial.println(rec.heading);
    Serial.println("-----");  
  } 
}
/***********************************************************/

void forwardWhiskers()
{
  unsigned long startingTime = millis();
  int oneCell = 1800; ///Milliseconds required to travel from one cell's center to another's center (adjust accordingly).

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
  maze.mouseColumn += neighboringCells[maze.mouseHeading][1]; //NEWLY ADDED, NOT TESTED!
 
  rec.row = maze.mouseRow;
  rec.column = maze.mouseColumn;
  rec.heading = maze.mouseHeading;
  ///updateStepCount(); ///new added
}




void calibrate()
{
  sensors.initialize();
  targetSide = (sensors.left + sensors.right)/2;

  motors.turn(RIGHT,90);
  sensors.initialize();
  targetFront = sensors.front;
  ///thresholdSide = (targetSide + sensors.left)/2; //Original
  thresholdSide = (targetSide + 3*sensors.left)/6;
  /**
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
    maze.addWalls((maze.mouseHeading + 3) % 5);
  }
}

void turnTowardBestNeighbor()
{
  byte desiredHeading = maze.findBestNeighbor();
  int difference = maze.mouseHeading - desiredHeading;

  if(difference == 1 || difference == -3)///left
  {
    motors.turn(LEFT,90);
  }
  else if(difference == -1 || difference == 3)///right
  {
    motors.turn(RIGHT,90);
  }
  else if(difference == -2 || difference == 2)///180 turn
  {
    motors.turn(LEFT,180);
  }

  maze.mouseHeading = desiredHeading;
}

byte state()
{
  ///int threshold = 50; //Test first
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
  case 1: ///Front sensor triggered
    ///For Demo
    motors.backward();
    delay(1000);
    ///End-For Demo
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
  case 2: ///Left Sensor
    motors.turn(RIGHT, 45);
    sensors.initialize();
    break;
  case 4: ///Right sensor
    motors.turn(LEFT, 45);
    sensors.initialize();
    break;
  case 7: ///All sensors
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