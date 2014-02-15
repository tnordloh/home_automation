// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
char date[12];
char time[9];

boolean DEBUG=false;
void setDateTime() {
  Serial.println("set date here");
  Serial.readBytes(date,11);
  date[11] = '\0';
  Serial.println(date);
  boolean timeentered=false;
  while (!timeentered) {
    delay(30);   
    //Serial.print(date);
    //Serial.println("currently stuck waiting for time");
    if(Serial.available()) {     
      int command = Serial.read();
      Serial.println(command);
      if(command==84) {
        timeentered=true;
        Serial.readBytes(time,8);
        time[8] = '\0';
        Serial.println(time);
        rtc.adjust(DateTime(date, time));
      }
    }
  }
}
void setup () {
  Serial.begin(2400);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}



void loop () {
  if (Serial.available()) {      // Look for char in serial que and process if found
    int command = Serial.read();
    Serial.println(command);
    if(command==68) {
    setDateTime();
//      boolean timeentered=false;
//      while (!timeentered) {
//        delay(3000);   
//        Serial.print(date);
//        Serial.println("currently stuck waiting for time");
//        if(Serial.available()) {     
//          command = Serial.read();
//          Serial.println(command);
//          if(command==84) {
//            timeentered=true;
//            Serial.readBytes(time,8);
//            time[8] = '\0';
//            Serial.println(time);
//            rtc.adjust(DateTime(date, time));
//          }
//        }
//      }
    }
  }
  DateTime now = rtc.now();
  //Serial.println(__DATE__);
  //Serial.println(__TIME__);
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  //Serial.print(" since midnight 1/1/1970 = ");
  //Serial.print(now.unixtime());
  //Serial.print("s = ");
  //Serial.print(now.unixtime() / 86400L);
  //Serial.println("d");
  if(DEBUG) {
    // calculate a date which is 7 days and 30 seconds into the future
    //DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    //Serial.print(" now + 7d + 30s: ");
    //Serial.print(future.year(), DEC);
    //Serial.print('/');
    //Serial.print(future.month(), DEC);
    //Serial.print('/');
    //Serial.print(future.day(), DEC);
    //Serial.print(' ');
    //Serial.print(future.hour(), DEC);
    //Serial.print(':');
    //Serial.print(future.minute(), DEC);
    //Serial.print(':');
    //Serial.print(future.second(), DEC);
    //Serial.println();
    //    
    //Serial.println();
  }
  delay(3000);
}


