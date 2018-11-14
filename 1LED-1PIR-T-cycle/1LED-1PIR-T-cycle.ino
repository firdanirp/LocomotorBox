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

int phase1_1 = 0;
int phase1_2 = 0;

int StartYear = 2018;
int StartMonth = 01;
int StartDate = 01;

int HourOn1_1 = 8;
int MinuteOn1_1 = 00;

int HourOn1_2 = 8;
int MinuteOn1_2 = 00;

int HourOn1_3 = 8;
int MinuteOn1_3 = 00;

int light1_1 = 0;
int light1_2 = 0;
int light1_3 = 0;

int dark1_1 = 0;
int dark1_2 = 0;
int dark1_3 = 0;

int Days1_1 = 1;
int Days1_2 = 1;
int Days1_3 = 1;

int T_Cycle1_1=24;
int T_Cycle1_2=24;
int T_Cycle1_3=24;

float RatioOn1_1=12;
float RatioOn1_2=12;
float RatioOn1_3=12;

float RatioOff1_1=12;
float RatioOff1_2=12;
float RatioOff1_3=12;

// Digital In-Out
int DIn1 = 2;  // PIR1
int DOut1 = 3; // LED1

// Global variables
int InitialFlag = 0;
int LightFlag1 = 0;

int TimeSet = 0;
int LightSet1 = 0;
int LightSet2 = 0;

int reset1_1 = 0;
int reset1_2 = 0;
int addref = 0;
float timeStart=0;
float timeEnd=0;
int hourstart = 0;
int minstart = 0;
int secstart = 0;
int Day=0;
float intTime=0;
float intTimeex=0;
float intHour=0;
float intMinute=0;
float intSecond=0;


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
  pinMode(DIn1, INPUT);    // PIR1
  pinMode(DOut1, OUTPUT);  // LED1

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
  if (Serial.available() == 23 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);
    //Box1
    StartYear = getInt(lightIn1.substring(0, 4));
    StartMonth = getInt(lightIn1.substring(4, 6));
    StartDate = getInt(lightIn1.substring(6, 8));
    HourOn1_1 = getInt(lightIn1.substring(8, 10));
    MinuteOn1_1 = getInt(lightIn1.substring(10, 12));
    T_Cycle1_1 = getInt(lightIn1.substring(12, 14));
    Days1_1 = getInt(lightIn1.substring(14, 17));
    RatioOn1_1 = getInt(lightIn1.substring(17, 19));
    RatioOff1_1 = getInt(lightIn1.substring(19, 21));
    dark1_1 = getInt(lightIn1.substring(21, 22));
    light1_1 = getInt(lightIn1.substring(22, 23));

    LightSet1 = 1;
  }


  // Phase2+3
  if (Serial.available() == 30 && InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 0)
  {
    lightIn1 = Serial.readString();
    Serial.println(lightIn1);
    //Box1
    HourOn1_2 = getInt(lightIn1.substring(0, 2));
    MinuteOn1_2 = getInt(lightIn1.substring(2, 4));
    T_Cycle1_2 = getInt(lightIn1.substring(4, 6));
    Days1_2 = getInt(lightIn1.substring(6, 9));
    RatioOn1_2 = getInt(lightIn1.substring(9, 11));
    RatioOff1_2 = getInt(lightIn1.substring(11, 13));
    dark1_2 = getInt(lightIn1.substring(13, 14));
    light1_2 = getInt(lightIn1.substring(14, 15));

    HourOn1_3 = getInt(lightIn1.substring(15, 17));
    MinuteOn1_3 = getInt(lightIn1.substring(17, 19));
    T_Cycle1_3 = getInt(lightIn1.substring(19, 21));
    Days1_3 = getInt(lightIn1.substring(21, 24));
    RatioOn1_3 = getInt(lightIn1.substring(24, 26));
    RatioOff1_3 = getInt(lightIn1.substring(26, 28));
    dark1_3 = getInt(lightIn1.substring(28, 29));
    light1_3 = getInt(lightIn1.substring(29, 30));

    LightSet2 = 1;
  }

  // Begin to print the headers and set light flag
  if (InitialFlag == 0 && TimeSet == 1 && LightSet1 == 1 && LightSet2 == 1)
  {
    Serial.println("HH:MM:SS MO/DY/YEAR DAY HH:MM:SS LED01 PIR01" );
    InitialFlag = 1;
  }

  if (InitialFlag == 1){
    // Calculate Internal Time based on External Time
    // Set the reference for Only for the first Day because we want to synchronize the first time point of internal and external clock
  if (reset1_1==0){ 
    clock.getTime();
    hourstart = clock.hour;
    minstart = clock.minute;
    secstart = clock.second;
    Day=Day+1;
    reset1_1=1;
  }
    // For the first day, we need to add the reference day into the elapse time to get the internal time
  if (reset1_1==1 && reset1_2==0){
    clock.getTime();
    setTime(clock.hour,clock.minute,clock.second,clock.dayOfMonth,clock.month,(clock.year+2000));
    timeStart = ((float)hourstart*60.*60.) + ((float)minstart*60.) + (float)secstart; // second of the day
    timeEnd =  ((float)hour() * 60. * 60.) + ((float)minute() * 60.) + (float)second(); // second of the day
    intTime = timeStart + ((timeEnd - timeStart)*(24./T_Cycle1_1));
  
    //Internal Time
    intSecond = second(intTime);
    intMinute = minute(intTime);
    intHour   = hour(intTime);
  }
  
    // From the second day, we dont need to add the reference into internal time anymore
  if (reset1_1==1 && reset1_2==1){
    clock.getTime();
    setTime(clock.hour,clock.minute,clock.second,clock.dayOfMonth,clock.month,(clock.year+2000));
    
    // Additional step to make sure that the elapsed time calculation is correct when external clock resets from 23:59 to 00:00
    if (addref ==0 && clock.hour==0&&clock.minute==0){
      hourstart==0;
      minstart==0;
      secstart==clock.second;
      addref=1;
    }
    // Before external clock resets
    if (addref==0){
    timeStart = ((float)hourstart*60.*60.) + ((float)minstart*60.) + (float)secstart; // second of the day
    timeEnd =  ((float)hour() * 60. * 60.) + ((float)minute() * 60.) + (float)second(); // second of the day
    intTime = (timeEnd - timeStart)*(24./T_Cycle1_1);
    intSecond = second(intTime);
    intMinute = minute(intTime);
    intHour   = hour(intTime);
    }
    // After external clock resets
    if (addref==1){
      timeStart = ((float)hourstart*60.*60.) + ((float)minstart*60.) + (float)secstart; // second of the day
      timeEnd =  ((float)hour() * 60. * 60.) + ((float)minute() * 60.) + (float)second(); // second of the day
      intTimeex = intTime + (timeEnd - timeStart)*(24./T_Cycle1_1);

      intSecond = second(intTimeex);
      intMinute = minute(intTimeex);
      intHour   = hour(intTimeex);

    }
    
  }
    //Reset the reference to clock time from the second Day at 23:59:00, after 1 min of PIR sampling, it becomes the next day
    if (intHour==23. && intMinute==59.){
      clock.getTime();
      hourstart = clock.hour;
      minstart = clock.minute;
      secstart = clock.second;
      Day=Day+1;
      reset1_2=1;
      addref=0;
  }
      
  
    // Only start recording when the Internal second = 0, otherwise stay in the (delay 1 milisec) loop
  while((intSecond==0)==false){
    delay(1);
    intTime=intTime+1.;
    intSecond = second(intTime);
    intMinute = minute(intTime);
    intHour   = hour(intTime);
    clock.getTime();
  }

    // End phase 1 when

    if (Day>Days1_1)
    {
      phase1_1 = 1;
    }

    // End phase 2 when

    if (Day>Days1_1+Days1_2)
    {
      phase1_2 = 1;
    }
    
    //////////For Phase1
    //Box1
    if (phase1_1 == 0)
    {
      if (intHour*60 +  intMinute >= HourOn1_1 * 60 + MinuteOn1_1 && (intHour*60 + intMinute) - (HourOn1_1 * 60 + MinuteOn1_1) <= 24 * (RatioOn1_1/(RatioOn1_1+RatioOff1_1)) * 60)
      {
        if (light1_1 == 0 && dark1_1 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
        if (light1_1 == 0 && dark1_1 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_1 == 1 && dark1_1 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
      }
      else
      {
        if (light1_1 == 0 && dark1_1 == 0)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_1 == 0 && dark1_1 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_1 == 1 && dark1_1 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
      }
    }
    
    //////////For Phase2
    if (phase1_1 == 1)
    {
      if (intHour*60 +  intMinute >= HourOn1_2 * 60 + MinuteOn1_2 && (intHour*60 + intMinute)-(HourOn1_2 * 60 + MinuteOn1_2) <= 24 * (RatioOn1_2/(RatioOn1_2+RatioOff1_2)) * 60)
      {
        if (light1_2 == 0 && dark1_2 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
        if (light1_2 == 0 && dark1_2 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_2 == 1 && dark1_2 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
      }
      else
      {
        if (light1_2 == 0 && dark1_2 == 0)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_2 == 0 && dark1_2 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_2 == 1 && dark1_2 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
      }
    }
    //////////For Phase3
    if (phase1_2 == 1)
    {
      if (intHour*60 +  intMinute >= HourOn1_3 * 60 + MinuteOn1_3 && (intHour*60 + intMinute)-(HourOn1_3 * 60 + MinuteOn1_3) <= 24 * (RatioOn1_3/(RatioOn1_3+RatioOff1_3)) * 60)
      {
        if (light1_3 == 0 && dark1_3 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
        if (light1_3 == 0 && dark1_3 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_3 == 1 && dark1_3 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
      }
      else
      {
        if (light1_3 == 0 && dark1_3 == 0)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_3 == 0 && dark1_3 == 1)
        {
          digitalWrite(DOut1, LOW);
          LightFlag1 = 0;
        }
        if (light1_3 == 1 && dark1_3 == 0)
        {
          digitalWrite(DOut1, HIGH);
          LightFlag1 = 1;
        }
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
  int mPIR1 = 0;

  // per-second sampling
  int PIR1 = digitalRead(DIn1);

  // sensor value sampling for 1-min
  for (int i = 0; i < (999/(24/T_Cycle1_1)); i++)
  {
    PIR1 = PIR1 + digitalRead(DIn1);

    delay(60); // sampling 200 times per minute
  }

  // 1-min summation
  mPIR1 = PIR1;

  // Outputs

  printTime();
  Serial.print(" ");

  //Box1
  Serial.print("0000");
  Serial.print(LightFlag1);
  Serial.print(" ");
  if(mPIR1<10000 && mPIR1>999)
  {
    Serial.print("0");
  }
  if(mPIR1<1000 && mPIR1>99)
  {
    Serial.print("00");
  }
  if(mPIR1<100 && mPIR1>9)
  {
    Serial.print("000");
  }
  if(mPIR1<10)
  {
    Serial.print("0000");
  }
  Serial.print(mPIR1);
  Serial.print(" ");

}

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
  Serial.print(" 00");
  Serial.print(Day, DEC);
  Serial.print(" ");
  Serial.print(intHour, 0);
  Serial.print(":");
  Serial.print(intMinute, 0);
  Serial.print(":");
  Serial.print(intSecond, 0);
}
