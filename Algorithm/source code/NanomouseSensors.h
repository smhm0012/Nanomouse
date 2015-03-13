#include <Arduino.h>

template <byte leftEmitter, byte leftDetector, byte frontEmitter, byte frontDetector, byte rightEmitter, byte rightDetector>

class NanomouseSensors
{
private:
  //Used to store sensor values when emitters are off
  int leftAmbient;
  int frontAmbient;
  int rightAmbient;
  //Used to store sensor values when emitters are on
  int leftCombined;
  int frontCombined;
  int rightCombined;
  //Used to filter out ambient light
  int leftReflected;
  int frontReflected;
  int rightReflected;

  //Variables used for smoothing
  int leftTotal;
  int frontTotal;
  int rightTotal;
  static const byte numReadings = 20; //Number 20 based on testings
  byte index;
  int leftReadings[numReadings];
  int frontReadings[numReadings];
  int rightReadings[numReadings];
  int leftSmoothed;
  int frontSmoothed;
  int rightSmoothed;

public:
  int left;
  int front;
  int right;

  void configure()
  {
    pinMode(leftEmitter, OUTPUT);
    pinMode(frontEmitter, OUTPUT);
    pinMode(rightEmitter, OUTPUT);
  }
  void sense()
  {
    digitalWrite(leftEmitter, HIGH);
    delay(1);
    leftCombined = analogRead(leftDetector);
    digitalWrite(leftEmitter, LOW);
    delay(1);
    leftAmbient = analogRead(leftDetector);
    leftReflected = leftCombined - leftAmbient;
    leftTotal -= leftReadings[index];
    leftReadings[index] = leftReflected;
    leftTotal += leftReadings[index];

    digitalWrite(frontEmitter, HIGH);
    delay(1); //1 milisecond
    frontCombined = analogRead(frontDetector);
    digitalWrite(frontEmitter, LOW);
    delay(1);
    frontAmbient = analogRead(frontDetector);
    frontReflected = frontCombined - frontAmbient;
    frontTotal -= frontReadings[index];
    frontReadings[index] = frontReflected;
    frontTotal += frontReadings[index];

    digitalWrite(rightEmitter, HIGH);
    delay(1);
    rightCombined = analogRead(rightDetector);
    digitalWrite(rightEmitter, LOW);
    delay(1);
    rightAmbient = analogRead(rightDetector);
    rightReflected = rightCombined - rightAmbient;
    rightTotal -= rightReadings[index];
    rightReadings[index] = rightReflected;
    rightTotal += rightReadings[index];

    index += 1;
    if (index >= numReadings){
      index = 0;
    }
    leftSmoothed = leftTotal/numReadings;
    frontSmoothed = frontTotal/numReadings;
    rightSmoothed = rightTotal/numReadings;

    left = leftSmoothed;
    front = frontSmoothed;
    right = rightSmoothed;
  }
  void view()
  {
    //Serial.print(analogRead(leftDetector));
    Serial.print(leftSmoothed);
    Serial.print("\t");
    //Serial.print(analogRead(frontDetector
    Serial.print(frontSmoothed);
    Serial.print("\t");
    //Serial.println(analogRead(rightDetector));
    Serial.println(rightSmoothed);
  }

  void initialize()
  {
    for(byte i = 0; i < numReadings;i++)
    {
      sense();
    }
  }
};


