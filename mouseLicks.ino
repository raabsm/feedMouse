#include <TimeAlarms.h>
#include <Time.h>
#include <TimeLib.h>

// USER DEFINE PARAMETERS
const int ttyl=8;
const int solenoid=7;
const int speakerPin = 13;
const int speakerFreq = 5000;
const int pulseLength = 1000;
const int solOpenTime = 15; //ms time the solenoid will be open and deliver water
const int solInterval = 200; //ms delay between 
const int toneLength = 100; //ms the tone plays
const long shortTime = 900000; //ms the solenoid is open for (food avalibility)
const long longTime = 3600000; //ms the solenoid is open for (food avalibility)
int conditionSequence[] = {1,2,1}; // 0: short food availability, toneA; 1: long food availability, toneB

// OTHER VARIABLES
int index = 4;
int repetitions;
unsigned long startTime;
const int TrialNum = sizeof(conditionSequence) / sizeof(conditionSequence[0]);

void setup(){
  randomizeSequence();
  Serial.begin(9600);
  setTime(9,00,0,1,1,16); // set time to Saturday 9:00am Jan 1 2016
  // create the alarms 
  Alarm.alarmRepeat(9,29,5, Trial);
  Alarm.alarmRepeat(12,29,45, Trial);
  Alarm.alarmRepeat(15,30,0, Trial);
  randomSeed(analogRead(4));
  pinMode(solenoid, OUTPUT);
  pinMode(ttyl,OUTPUT);
}

void loop(){
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

void Trial(){
 if (index>TrialNum){
    randomizeSequence();
    index = 0;
  delay(100*random(10, 20));
  digitalWrite(ttyl,HIGH);
  
  switch(conditionSequence[index]){
    // SHORT FOOD AVAILABILITY CONDITION
    case 1:
      tone(speakerPin, speakerFreq, toneLength);
      delay(toneLength);
      repetitions = (shortTime)/(solOpenTime+solInterval);
      break;
    // LONG FOOD AVAILABILITY CONDITION
    case 2:
      startTime = millis();
      while ((unsigned long)(millis() - startTime)<toneLength){
        tone(speakerPin, speakerFreq, pulseLength);
        delay(pulseLength*2);
      }
      repetitions = (shortTime)/(solOpenTime+solInterval);
      break;
  }
  // START FOOD AVAILABILITY
  for (int thistime = 0; thistime<repetitions; thistime++){
    digitalWrite(solenoid, HIGH);
    delay(solOpenTime);
    digitalWrite(solenoid, LOW);
    delay(solInterval);
  }
  digitalWrite(ttyl,LOW);
  
  index++;
  if (index>TrialNum){
    randomizeSequence();
    index = 0;
  }
}

void randomizeSequence(){
  for (int i = 0; i < TrialNum - 1; i++){
    int j = random(1, TrialNum - i);
    int t = conditionSequence[i];
    conditionSequence[i] = conditionSequence[j];
    conditionSequence[j] = t;
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10){Serial.print('0');}
  Serial.print(digits);
}

