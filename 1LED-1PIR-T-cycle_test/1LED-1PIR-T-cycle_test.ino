/*
 * 1-LED, 1-PIR box driver (1 arduino driving 10 box)
 * 
 * Jihwan Myung 
 * 14 Oct 2018 PIR range increased [0, 1000]
 * 30 Sep 2018 
 * 29 Sep 2018 (initial)
 * 
 * Vuong Truong
 * 9 Oct 2018 make the GUI
 * 5 Oct 2018 complete the code
 * 3 Oct 2018 (major revision)
 * 
 */

#include <Wire.h>
#include "DS1307.h"
#include <Time.h>
#include <TimeLib.h>

DS1307 clock; //define a object of DS1307 class

String dateIn;
String lightIn1;

// declaration in array of 10 boxes
int PIR[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int mPIR[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int HourOn1[10] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8}; // phase 1
int MinuteOn1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int HourOn2[10] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8}; // phase 2
int MinuteOn2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int HourOn3[10] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8}; // phase 3
int MinuteOn3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int light1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // phase 1
int light2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // phase 2
int light3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // phase 3
int dark1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // phase 1
int dark2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // phase 2
int dark3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // phase 3

float RatioOn1[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
float RatioOn2[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
float RatioOn3[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

float RatioOff1[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
float RatioOff2[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};
float RatioOff3[10] = {12, 12, 12, 12, 12, 12, 12, 12, 12, 12};

// Digital In-Out
int DIn[10] = {2, 4, 6, 8, 10, 12, 22, 24, 26, 28};  // PIR
int DOut[10] = {3, 5, 7, 9, 11, 13, 23, 25, 27, 29}; // LED

// Global variables
int phase1 = 0;
int phase2 = 0;

int StartHour=0;
int StartMinute=0;
int StartYear = 2018;
int StartMonth = 01;
int StartDate = 01;

float T_Cycle = 24;
float T_Cycle1 = 24;
float T_Cycle2 = 24;
float T_Cycle3 = 24;

int LightFlag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int TimeSet = 0;
int LightSet1 = 0;
int LightSet2 = 0;
int LightSet3 = 0;
int LightSet4 = 0;
int LightSet5 = 0;
int LightSet6 = 0;

int InitialFlag = 0;

int reset1 = 0;
int reset2 = 0;
int addref = 0;

float timeStart = 0.;
float timeEnd = 0.;
float hourstart = 0.;
float minstart = 0.;
float secstart = 0.;
int Day = 0;
int Days1 = 0;
int Days2 = 0;
int Days3 = 0;
float intTime = 0.;
float intTimeex = 0.; //For re-reference when the external clock turns 00:00:00
float intHour = 0.;
float intMinute = 0.;
float intSecond = 0.;

// Define a function to convert string to integer
int getInt(String text)
{
  char temp[6];
  text.toCharArray(temp, 5);
  int x = atoi(temp);
  return x;
}

//////////////////////////////////////////////////////////////////////////////////////// Set up run
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  for (int i = 0; i < 10; i++)
  {
    pinMode(DIn[i], INPUT);   // PIR
    pinMode(DOut[i], OUTPUT); // LED
  }

  delay(1000); // recommended delay before start-up (1-sec)
}

//////////////////////////////////////////////////////////////////////////////////////// Main loop
void loop()
{

  //Time Synching (Get the input from computer time through Python interface)
  if (Serial.available() == 19 && TimeSet == 0)
  {
    dateIn = Serial.readString();
    clock.stopClock();
    clock.fillByYMD(getInt(dateIn.substring(0, 4)), getInt(dateIn.substring(5, 7)), getInt(dateIn.substring(8, 10)));
    clock.fillByHMS(getInt(dateIn.substring(11, 13)), getInt(dateIn.substring(14, 16)), getInt(dateIn.substring(17, 19)));
    clock.setTime();
    clock.startClock();

    Serial.println(dateIn);

    TimeSet = 1;
  }
  ////////////////////// Light Schedule (Get the input from Python interface)

  // Phase1
  if (Serial.available() == 27 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);

    StartHour = getInt(lightIn1.substring(0, 2));
    StartMinute = getInt(lightIn1.substring(2, 4));
    StartYear = getInt(lightIn1.substring(4, 8));
    StartMonth = getInt(lightIn1.substring(8, 10));
    StartDate = getInt(lightIn1.substring(10, 12));
    Days1 = getInt(lightIn1.substring(12, 15));
    Days2 = getInt(lightIn1.substring(15, 18));
    Days3 = getInt(lightIn1.substring(18, 21));
    T_Cycle1 = getInt(lightIn1.substring(21, 23));
    T_Cycle2 = getInt(lightIn1.substring(23, 25));
    T_Cycle3 = getInt(lightIn1.substring(25, 27));

    LightSet1 = 1;
  }
  if (Serial.available() == 60 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);
    dark1[0] = getInt(lightIn1.substring(0, 1));
    light1[0] = getInt(lightIn1.substring(1, 2));
    dark1[1] = getInt(lightIn1.substring(2, 3));
    light1[1] = getInt(lightIn1.substring(3, 4));
    dark1[2] = getInt(lightIn1.substring(4, 5));
    light1[2] = getInt(lightIn1.substring(5, 6));
    dark1[3] = getInt(lightIn1.substring(6, 7));
    light1[3] = getInt(lightIn1.substring(7, 8));
    dark1[4] = getInt(lightIn1.substring(8, 9));
    light1[4] = getInt(lightIn1.substring(9, 10));
    dark1[5] = getInt(lightIn1.substring(10, 11));
    light1[5] = getInt(lightIn1.substring(11, 12));
    dark1[6] = getInt(lightIn1.substring(12, 13));
    light1[6] = getInt(lightIn1.substring(13, 14));
    dark1[7] = getInt(lightIn1.substring(14, 15));
    light1[7] = getInt(lightIn1.substring(15, 16));
    dark1[8] = getInt(lightIn1.substring(16, 17));
    light1[8] = getInt(lightIn1.substring(17, 18));
    dark1[9] = getInt(lightIn1.substring(18, 19));
    light1[9] = getInt(lightIn1.substring(19, 20));

    dark2[0] = getInt(lightIn1.substring(20, 21));
    light2[0] = getInt(lightIn1.substring(21, 22));
    dark2[1] = getInt(lightIn1.substring(22, 23));
    light2[1] = getInt(lightIn1.substring(23, 24));
    dark2[2] = getInt(lightIn1.substring(24, 25));
    light2[2] = getInt(lightIn1.substring(25, 26));
    dark2[3] = getInt(lightIn1.substring(26, 27));
    light2[3] = getInt(lightIn1.substring(27, 28));
    dark2[4] = getInt(lightIn1.substring(28, 29));
    light2[4] = getInt(lightIn1.substring(29, 30));
    dark2[5] = getInt(lightIn1.substring(30, 31));
    light2[5] = getInt(lightIn1.substring(31, 32));
    dark2[6] = getInt(lightIn1.substring(32, 33));
    light2[6] = getInt(lightIn1.substring(33, 34));
    dark2[7] = getInt(lightIn1.substring(34, 35));
    light2[7] = getInt(lightIn1.substring(35, 36));
    dark2[8] = getInt(lightIn1.substring(36, 37));
    light2[8] = getInt(lightIn1.substring(37, 38));
    dark2[9] = getInt(lightIn1.substring(38, 39));
    light2[9] = getInt(lightIn1.substring(39, 40));

    dark3[0] = getInt(lightIn1.substring(40, 41));
    light3[0] = getInt(lightIn1.substring(41, 42));
    dark3[1] = getInt(lightIn1.substring(42, 43));
    light3[1] = getInt(lightIn1.substring(43, 44));
    dark3[2] = getInt(lightIn1.substring(44, 45));
    light3[2] = getInt(lightIn1.substring(45, 46));
    dark3[3] = getInt(lightIn1.substring(46, 47));
    light3[3] = getInt(lightIn1.substring(47, 48));
    dark3[4] = getInt(lightIn1.substring(48, 49));
    light3[4] = getInt(lightIn1.substring(49, 50));
    dark3[5] = getInt(lightIn1.substring(50, 51));
    light3[5] = getInt(lightIn1.substring(51, 52));
    dark3[6] = getInt(lightIn1.substring(52, 53));
    light3[6] = getInt(lightIn1.substring(53, 54));
    dark3[7] = getInt(lightIn1.substring(54, 55));
    light3[7] = getInt(lightIn1.substring(55, 56));
    dark3[8] = getInt(lightIn1.substring(56, 57));
    light3[8] = getInt(lightIn1.substring(57, 58));
    dark3[9] = getInt(lightIn1.substring(58, 59));
    light3[9] = getInt(lightIn1.substring(59, 60));

    LightSet2 = 1;
  }

  if (Serial.available() == 60 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1 && LightSet3 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);

    HourOn1[0] = getInt(lightIn1.substring(0, 2));
    MinuteOn1[0] = getInt(lightIn1.substring(2, 4));
    HourOn1[1] = getInt(lightIn1.substring(4, 6));
    MinuteOn1[1] = getInt(lightIn1.substring(6, 8));
    HourOn1[2] = getInt(lightIn1.substring(8, 10));
    MinuteOn1[2] = getInt(lightIn1.substring(10, 12));
    HourOn1[3] = getInt(lightIn1.substring(12, 14));
    MinuteOn1[3] = getInt(lightIn1.substring(14, 16));
    HourOn1[4] = getInt(lightIn1.substring(16, 18));
    MinuteOn1[4] = getInt(lightIn1.substring(18, 20));
    HourOn1[5] = getInt(lightIn1.substring(20, 22));
    MinuteOn1[5] = getInt(lightIn1.substring(22, 24));
    HourOn1[6] = getInt(lightIn1.substring(24, 26));
    MinuteOn1[6] = getInt(lightIn1.substring(26, 28));
    HourOn1[7] = getInt(lightIn1.substring(28, 30));
    MinuteOn1[7] = getInt(lightIn1.substring(30, 32));
    HourOn1[8] = getInt(lightIn1.substring(32, 34));
    MinuteOn1[8] = getInt(lightIn1.substring(34, 36));
    HourOn1[9] = getInt(lightIn1.substring(36, 38));
    MinuteOn1[9] = getInt(lightIn1.substring(38, 40));

    RatioOn1[0] = getInt(lightIn1.substring(40, 42));
    RatioOff1[0] = getInt(lightIn1.substring(42, 44));
    RatioOn1[1] = getInt(lightIn1.substring(44, 46));
    RatioOff1[1] = getInt(lightIn1.substring(46, 48));
    RatioOn1[2] = getInt(lightIn1.substring(48, 50));
    RatioOff1[2] = getInt(lightIn1.substring(50, 52));
    RatioOn1[3] = getInt(lightIn1.substring(52, 54));
    RatioOff1[3] = getInt(lightIn1.substring(54, 56));
    RatioOn1[4] = getInt(lightIn1.substring(56, 58));
    RatioOff1[4] = getInt(lightIn1.substring(58, 60));

    LightSet3 = 1;
    //Serial.println(HourOn1[5]);
    //Serial.println(MinuteOn1[5]);
  }
  if (Serial.available() == 60 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1 && LightSet3 == 1 && LightSet4 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);

    RatioOn1[5] = getInt(lightIn1.substring(0, 2));
    RatioOff1[5] = getInt(lightIn1.substring(2, 4));
    RatioOn1[6] = getInt(lightIn1.substring(4, 6));
    RatioOff1[6] = getInt(lightIn1.substring(6, 8));
    RatioOn1[7] = getInt(lightIn1.substring(8, 10));
    RatioOff1[7] = getInt(lightIn1.substring(10, 12));
    RatioOn1[8] = getInt(lightIn1.substring(12, 14));
    RatioOff1[8] = getInt(lightIn1.substring(14, 16));
    RatioOn1[9] = getInt(lightIn1.substring(16, 18));
    RatioOff1[9] = getInt(lightIn1.substring(18, 20));

    HourOn2[0] = getInt(lightIn1.substring(20, 22));
    MinuteOn2[0] = getInt(lightIn1.substring(22, 24));
    HourOn2[1] = getInt(lightIn1.substring(24, 26));
    MinuteOn2[1] = getInt(lightIn1.substring(26, 28));
    HourOn2[2] = getInt(lightIn1.substring(28, 30));
    MinuteOn2[2] = getInt(lightIn1.substring(30, 32));
    HourOn2[3] = getInt(lightIn1.substring(32, 34));
    MinuteOn2[3] = getInt(lightIn1.substring(34, 36));
    HourOn2[4] = getInt(lightIn1.substring(36, 38));
    MinuteOn2[4] = getInt(lightIn1.substring(38, 40));
    HourOn2[5] = getInt(lightIn1.substring(40, 42));
    MinuteOn2[5] = getInt(lightIn1.substring(42, 44));
    HourOn2[6] = getInt(lightIn1.substring(44, 46));
    MinuteOn2[6] = getInt(lightIn1.substring(46, 48));
    HourOn2[7] = getInt(lightIn1.substring(48, 50));
    MinuteOn2[7] = getInt(lightIn1.substring(50, 52));
    HourOn2[8] = getInt(lightIn1.substring(52, 54));
    MinuteOn2[8] = getInt(lightIn1.substring(54, 56));
    HourOn2[9] = getInt(lightIn1.substring(56, 58));
    MinuteOn2[9] = getInt(lightIn1.substring(58, 60));

    LightSet4 = 1;
    //Serial.println(RatioOn1[5]);
    //Serial.println(RatioOff1[5]);
  }
  if (Serial.available() == 60 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1 && LightSet3 == 1 && LightSet4 == 1 && LightSet5 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);

    RatioOn2[0] = getInt(lightIn1.substring(0, 2));
    RatioOff2[0] = getInt(lightIn1.substring(2, 4));
    RatioOn2[1] = getInt(lightIn1.substring(4, 6));
    RatioOff2[1] = getInt(lightIn1.substring(6, 8));
    RatioOn2[2] = getInt(lightIn1.substring(8, 10));
    RatioOff2[2] = getInt(lightIn1.substring(10, 12));
    RatioOn2[3] = getInt(lightIn1.substring(12, 14));
    RatioOff2[3] = getInt(lightIn1.substring(14, 16));
    RatioOn2[4] = getInt(lightIn1.substring(16, 18));
    RatioOff2[4] = getInt(lightIn1.substring(18, 20));
    RatioOn2[5] = getInt(lightIn1.substring(20, 22));
    RatioOff2[5] = getInt(lightIn1.substring(22, 24));
    RatioOn2[6] = getInt(lightIn1.substring(24, 26));
    RatioOff2[6] = getInt(lightIn1.substring(26, 28));
    RatioOn2[7] = getInt(lightIn1.substring(28, 30));
    RatioOff2[7] = getInt(lightIn1.substring(30, 32));
    RatioOn2[8] = getInt(lightIn1.substring(32, 34));
    RatioOff2[8] = getInt(lightIn1.substring(34, 36));
    RatioOn2[9] = getInt(lightIn1.substring(36, 38));
    RatioOff2[9] = getInt(lightIn1.substring(38, 40));

    HourOn3[0] = getInt(lightIn1.substring(40, 42));
    MinuteOn3[0] = getInt(lightIn1.substring(42, 44));
    HourOn3[1] = getInt(lightIn1.substring(44, 46));
    MinuteOn3[1] = getInt(lightIn1.substring(46, 48));
    HourOn3[2] = getInt(lightIn1.substring(48, 50));
    MinuteOn3[2] = getInt(lightIn1.substring(50, 52));
    HourOn3[3] = getInt(lightIn1.substring(52, 54));
    MinuteOn3[3] = getInt(lightIn1.substring(54, 56));
    HourOn3[4] = getInt(lightIn1.substring(56, 58));
    MinuteOn3[4] = getInt(lightIn1.substring(58, 60));

    LightSet5 = 1;
  }
  if (Serial.available() == 60 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1 && LightSet3 == 1 && LightSet4 == 1 && LightSet5 == 1 && LightSet6 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);

    HourOn3[5] = getInt(lightIn1.substring(0, 2));
    MinuteOn3[5] = getInt(lightIn1.substring(2, 4));
    HourOn3[6] = getInt(lightIn1.substring(4, 6));
    MinuteOn3[6] = getInt(lightIn1.substring(6, 8));
    HourOn3[7] = getInt(lightIn1.substring(8, 10));
    MinuteOn3[7] = getInt(lightIn1.substring(10, 12));
    HourOn3[8] = getInt(lightIn1.substring(12, 14));
    MinuteOn3[8] = getInt(lightIn1.substring(14, 16));
    HourOn3[9] = getInt(lightIn1.substring(16, 18));
    MinuteOn3[9] = getInt(lightIn1.substring(18, 20));

    RatioOn3[0] = getInt(lightIn1.substring(20, 22));
    RatioOff3[0] = getInt(lightIn1.substring(22, 24));
    RatioOn3[1] = getInt(lightIn1.substring(24, 26));
    RatioOff3[1] = getInt(lightIn1.substring(26, 28));
    RatioOn3[2] = getInt(lightIn1.substring(28, 30));
    RatioOff3[2] = getInt(lightIn1.substring(30, 32));
    RatioOn3[3] = getInt(lightIn1.substring(32, 34));
    RatioOff3[3] = getInt(lightIn1.substring(34, 36));
    RatioOn3[4] = getInt(lightIn1.substring(36, 38));
    RatioOff3[4] = getInt(lightIn1.substring(38, 40));
    RatioOn3[5] = getInt(lightIn1.substring(40, 42));
    RatioOff3[5] = getInt(lightIn1.substring(42, 44));
    RatioOn3[6] = getInt(lightIn1.substring(44, 46));
    RatioOff3[6] = getInt(lightIn1.substring(46, 48));
    RatioOn3[7] = getInt(lightIn1.substring(48, 50));
    RatioOff3[7] = getInt(lightIn1.substring(50, 52));
    RatioOn3[8] = getInt(lightIn1.substring(52, 54));
    RatioOff3[8] = getInt(lightIn1.substring(54, 56));
    RatioOn3[9] = getInt(lightIn1.substring(56, 58));
    RatioOff3[9] = getInt(lightIn1.substring(58, 60));

    LightSet6 = 1;
  }
  // Begin to print the headers and set light flag
  if (InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1 && LightSet3 == 1 && LightSet4 == 1 && LightSet5 == 1 && LightSet6 == 1)
  {
    Serial.println("HH:MM:SS MO/DY/YEAR DAY Internal LED01 PIR01 LED02 PIR02 LED03 PIR03 LED04 PIR04 LED05 PIR05 LED06 PIR06 LED07 PIR07 LED08 PIR08 LED09 PIR09 LED10 PIR10");
    InitialFlag = 1;
    T_Cycle = T_Cycle1;
    clock.getTime();
    while ((clock.hour==StartHour && clock.minute==StartMinute && clock.dayOfMonth==StartDate && clock.month == StartMonth && (clock.year+2000) == StartYear)==false){
      delay(1);
      clock.getTime();   
    }    
  }

  if (InitialFlag == 1)
  { clock.getTime();
    while ((clock.second==0)==false){
      delay(1);
      clock.getTime();
    }

    // Calculate Internal Time based on External Time
    // Set the reference for Only for the first Day because we want to synchronize the first time point of internal and external clock
    if (reset1 == 0)
    {
      hourstart = clock.hour;
      minstart = clock.minute;
      secstart = clock.second;
      Day = Day + 1;
      reset1 = 1;
    }
    // For the first day, we need to add the reference day into the elapse time to get the internal time
    if (reset1 == 1 && reset2 == 0)
    {
      setTime(clock.hour, clock.minute, clock.second, clock.dayOfMonth, clock.month, (clock.year + 2000));
      timeStart = (hourstart * 60. * 60.) + (minstart * 60.) + secstart; // second of the day
      timeEnd = hour() * 60. * 60. + minute() * 60. + second();          // second of the day
      intTime = timeStart + ((timeEnd - timeStart) * (24. / T_Cycle));

      //Internal Time
      intSecond = second(intTime);
      intMinute = minute(intTime);
      intHour = hour(intTime);
    }

    // From the second day, we dont need to add the reference into internal time anymore
    if (reset1 == 1 && reset2 == 1)
    {
      setTime(clock.hour, clock.minute, clock.second, clock.dayOfMonth, clock.month, (clock.year + 2000));

      // Additional step to make sure that the elapsed time calculation is correct when external clock resets from 23:59 to 00:00
      if (addref == 0 && clock.hour == 0 && clock.minute == 0 && clock.second == 0)
      {
        hourstart = 0;
        minstart = 0;
        secstart = clock.second;
        addref = 1;
      }
      // Before external clock resets
      if (addref == 0)
      {
        timeStart = ((float)hourstart * 60. * 60.) + ((float)minstart * 60.) + (float)secstart; // second of the day
        timeEnd = ((float)hour() * 60. * 60.) + ((float)minute() * 60.) + (float)second();      // second of the day
        intTime = (timeEnd - timeStart) * (24. / T_Cycle);
        intSecond = second(intTime);
        intMinute = minute(intTime);
        intHour = hour(intTime);
      }
      // After external clock resets
      if (addref == 1)
      {
        timeStart = ((float)hourstart * 60. * 60.) + ((float)minstart * 60.) + (float)secstart; // second of the day
        timeEnd = ((float)hour() * 60. * 60.) + ((float)minute() * 60.) + (float)second();      // second of the day
        intTimeex = intTime + 60. * (24. / T_Cycle) + (timeEnd - timeStart) * (24. / T_Cycle);

        intSecond = second(intTimeex);
        intMinute = minute(intTimeex);
        intHour = hour(intTimeex);

      }
    }
    //Reset the reference to clock time from the second Day at 23:59:00, after 1 min of PIR sampling, it becomes the next day
    if (reset1==1 && intHour == 0. && intMinute < (24/T_Cycle) )
    {
      hourstart = clock.hour;
      minstart = clock.minute;
      secstart = clock.second;
      Day = Day + 1;
      reset2 = 1;
      addref = 0;
    }

    // End phase 1 when

    if (Day > Days1)
    {
      phase1 = 1;
      T_Cycle = T_Cycle2;
    }

    // End phase 2 when

    if (Day > Days1 + Days2)
    {
      phase2 = 1;
      T_Cycle = T_Cycle3;
    }

    //////////For Phase1
    //Box1
    if (phase1 == 0)
    {
      for (int i = 0; i < 10; i++)
      {
        //Box1
        if (intHour * 60 + intMinute >= HourOn1[i] * 60 + MinuteOn1[i] && (intHour * 60 + intMinute) - (HourOn1[i] * 60 + MinuteOn1[i]) <= 24 * (RatioOn1[i] / (RatioOn1[i] + RatioOff1[i])) * 60)
        {
          if (light1[i] == 0 && dark1[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
          if (light1[i] == 0 && dark1[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light1[i] == 1 && dark1[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        }
        else
        {
          if (light1[i] == 0 && dark1[i] == 0)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light1[i] == 0 && dark1[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light1[i] == 1 && dark1[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        }
      }
    }

    //////////For Phase2
    if (phase1 == 1)
    {
      for (int i = 0; i < 10; i++)
      {
        if (intHour * 60 + intMinute >= HourOn2[i] * 60 + MinuteOn2[i] && (intHour * 60 + intMinute) - (HourOn2[i] * 60 + MinuteOn2[i]) <= 24 * (RatioOn2[i] / (RatioOn2[i] + RatioOff2[i])) * 60)
        {
          if (light2[i] == 0 && dark2[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
          if (light2[i] == 0 && dark2[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light2[i] == 1 && dark2[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        } // if time condition is met
        else
        {
          if (light2[i] == 0 && dark2[i] == 0)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light2[i] == 0 && dark2[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light2[i] == 1 && dark2[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        }
      }
    }
    //////////For Phase3
    if (phase2 == 1)
    {
      for (int i = 0; i < 10; i++)
      {
        if (intHour * 60 + intMinute >= HourOn3[i] * 60 + MinuteOn3[i] && (intHour * 60 + intMinute) - (HourOn3[i] * 60 + MinuteOn3[i]) <= 24 * (RatioOn3[i] / (RatioOn3[i] + RatioOff3[i])) * 60)
        {
          if (light3[i] == 0 && dark3[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
          if (light3[i] == 0 && dark3[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light3[i] == 1 && dark3[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        } // if time condition is met
        else
        {
          if (light3[i] == 0 && dark3[i] == 0)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light3[i] == 0 && dark3[i] == 1)
          {
            digitalWrite(DOut[i], LOW);
            LightFlag[i] = 0;
          }
          if (light3[i] == 1 && dark3[i] == 0)
          {
            digitalWrite(DOut[i], HIGH);
            LightFlag[i] = 1;
          }
        } //else
      }
    }
    // If the after all of the settings (Time, Light, Initial header printing, Check schedule) begin to print time stamps and measurements

    printMeasurement();
    Serial.println(" ");
  }
}

// Define a function to print measurement
void printMeasurement()
{
  // mean values over 1-min
  for (int i = 0; i < 10; i++)
  {
    mPIR[i] = 0;
  }

  // per-second sampling
  for (int i = 0; i < 10; i++)
  {
    PIR[i] = digitalRead(DIn[i]);
  }

  // sensor value sampling for 1-min
  for (int i = 0; i < 990 ; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      PIR[j] = PIR[j] + digitalRead(DIn[j]);
    }

    delay(60); // sampling 1000 times per minute
  }

  // 1-min summation
  for (int i = 0; i < 10; i++)
  {
    mPIR[i] = PIR[i];
  }

  // Outputs

  printTime();
  Serial.print(" ");

  for (int i = 0; i < 10; i++)
  {
    Serial.print("0000");
    Serial.print(LightFlag[i]);
    Serial.print(" ");
    if (mPIR[i] < 10000 && mPIR[i] > 999)
    {
      Serial.print("0");
    }
    if (mPIR[i] < 1000 && mPIR[i] > 99)
    {
      Serial.print("00");
    }
    if (mPIR[i] < 100 && mPIR[i] > 9)
    {
      Serial.print("000");
    }
    if (mPIR[i] < 10)
    {
      Serial.print("0000");
    }
    Serial.print(mPIR[i]);
    Serial.print(" ");
  }
} // void

// Define a function to print time
void printTime()
{
  if (clock.hour < 10)
  {
    Serial.print("0");
  }
  Serial.print(clock.hour, DEC);
  Serial.print(":");
  if (clock.minute < 10)
  {
    Serial.print("0");
  }
  Serial.print(clock.minute, DEC);
  Serial.print(":");
  if (clock.second < 10)
  {
    Serial.print("0");
  }
  Serial.print(clock.second, DEC);
  Serial.print(" ");
  if (clock.month < 10)
  {
    Serial.print("0");
  }
  Serial.print(clock.month, DEC);
  Serial.print("/");
  if (clock.dayOfMonth < 10)
  {
    Serial.print("0");
  }
  Serial.print(clock.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(clock.year + 2000, DEC);
  Serial.print(" ");

  if (Day < 10)
  {
    Serial.print("00");
  }
  if (Day < 1000 && Day > 99)
  {
    Serial.print("0");
  }
  Serial.print(Day);
  Serial.print(" ");

  if (intHour < 10)
  {
    Serial.print("0");
  }
  Serial.print(intHour, 0);
  Serial.print(":");
  if (intMinute < 10)
  {
    Serial.print("0");
  }
  Serial.print(intMinute, 0);
  Serial.print(":");
  if (intSecond < 10)
  {
    Serial.print("0");
  }
  Serial.print(intSecond, 0);
}
