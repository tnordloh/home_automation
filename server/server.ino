// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
//virtualwire function, to support wireless connections between arduino units
#include <VirtualWire.h>  

RTC_DS1307 rtc;
char date[12];
char time[9];
int counter=0;

boolean DEBUG=false;
void setDateTime() {
  Serial.println("set date here");
  Serial.readBytes(date,11);
  date[11] = '\0'; //add \0 to end of string, otherwise, we read off the end of the array 
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
        time[8] = '\0'; //add \0 to end of string, otherwise, we read off the end of the array 
        Serial.println(time);
        rtc.adjust(DateTime(date, time));
      }
    }
  }
}
void setup () {
  Serial.begin(9600);

  vw_set_ptt_inverted(true); // Required for RF Link module
  vw_setup(2000);                 // Bits per sec
  vw_set_tx_pin(9);                // pin 9 is used as the transmit data out into the TX Link module, change this to suit your needs.
  vw_set_rx_pin(11);           // We will be receiving on pin 23 (Mega) ie the RX pin from the module connects to this pin.
  vw_rx_start();
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
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  Serial.println("receiving");

  if (vw_get_message(buf, &buflen)) // check to see if anything has been received
  {
    char received[buflen];
    //int i;
    // Message with a good checksum received.
    char temp=0;  
    for (int i = 0; i < buflen; i++) {   
      temp=(char)buf[i];
      received[i]=temp;
      Serial.print(temp);  // the received data is stored in buffer
    }
    Serial.print(" ");
    Serial.print(counter++);
    Serial.println();
    if(received[0]=='T' && received[1]=='I' && received[2]=='M' && received[3] == 'E') {
      delay(500);
      DateTime now = rtc.now();
      unsigned long now_unix = now.unixtime();
      Serial.println("request made to broadcast current time");
      Serial.println(now_unix);
      Serial.println(now_unix, HEX);
      char msg[50];
      sprintf(msg, "TR%lu", now_unix); 
      Serial.println( msg );

      Serial.println(msg);
      for(int i = 0;i<100;i++) {
        vw_send((uint8_t *)msg, strlen(msg));
        vw_wait_tx();          // Wait for message to finish
        Serial.println("sending time response");
        for(int j = 0; j < 6; j++) {
          Serial.print(byte(msg[j]));
          Serial.print('|');
        }
        Serial.println();
        delay(1000);
      }

    } 
  }

  if (Serial.available()) {      // Look for char in serial que and process if found
    int command = Serial.read();
    Serial.println(command);
    if(command==68) {
      setDateTime();
    }
  }
  DateTime now = rtc.now();
  //Serial.println(__DATE__);  //uncomment if I forget the format for entering in time
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
  Serial.println(now.unixtime());
  //Serial.print("s = ");
  //Serial.print(now.unixtime() / 86400L);
  //Serial.println("d");
  //if(DEBUG) {
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
  //}
  delay(3000);
}





