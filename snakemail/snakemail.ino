
//include the ability to write to eeprom, where we will store the last time the snake has been fed.
#include <EEPROM.h>

// RF Link using VirtualWire to receive messages
// simplex (one-way) receiver with a 315MHz RF Link Receiver module
// rx pin 23 on mega
#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder

//water sensor related pins
#define watersensor A0  
#define led 4
#define watersensorpower 3  //used to turn water sensor on and off, so that we dont continually electrify the water.

//button, press it when snakes are fed to start an appropriate timer.
#define button 5

//declare clock
unsigned long lastgoodtime=0;
unsigned long millis_at_lastgoodtime=0;
unsigned long last_snake_feeding=0;
unsigned long next_feeding_time_persistent=0;
int counter = 0;
int watersensorstate=0;
int watersensorcounter=0; 
boolean set_feeding_time=false;
byte prep_for_eeprom[4];
int leds[6] = {
  6,7,8,2,12,13};

void setup()
{
  Serial.begin(9600);    

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);                   // Bits per sec
  vw_set_rx_pin(11);           // We will be receiving on pin 11.
  vw_set_tx_pin(9);                // pin 9 is used as the transmit data out into the TX Link module, change this to suit your needs.
  vw_rx_start();                      // Start the receiver
  pinMode(watersensor,INPUT);
  pinMode(button,INPUT);

  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);

  pinMode(watersensorpower,OUTPUT);
  digitalWrite(watersensorpower,LOW);
  for(int i = 0; i < 6; i++) {
    pinMode(leds[i],OUTPUT);
    digitalWrite(leds[i],LOW);
  }
  requestTime();
  for(int i = 0 ; i < 4; i++) {
    prep_for_eeprom[i] = EEPROM.read(i);
    //Serial.println("SETTING PERSISTENT TIME:");
    Serial.print(prep_for_eeprom[i],BIN);
  }
  next_feeding_time_persistent = 0;
  next_feeding_time_persistent += (long)prep_for_eeprom[0] <<24;
  //Serial.println("Time after first shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  next_feeding_time_persistent += (long)prep_for_eeprom[1] <<16;
  //Serial.println("Time after second shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  next_feeding_time_persistent += (long)prep_for_eeprom[2] <<8;
  //Serial.println("Time after third shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  next_feeding_time_persistent += (long)prep_for_eeprom[3] ;
  //Serial.println("Time after fourth shift-insert:");
  //Serial.println(next_feeding_time_persistent);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  watersensorcounter++;
  //  for(int i = 0 ; i < 4; i++) {
  //    prep_for_eeprom[i] = EEPROM.read(i);
  //    Serial.println("SETTING PERSISTENT TIME:");
  //    Serial.println(prep_for_eeprom[i],BIN);
  //  }
  //  next_feeding_time_persistent=0;
  //  next_feeding_time_persistent += (long)prep_for_eeprom[0] <<24;
  //Serial.println("Time after first shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  //  next_feeding_time_persistent += (long)prep_for_eeprom[1] <<16;
  //Serial.println("Time after second shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  //  next_feeding_time_persistent += (long)prep_for_eeprom[2] <<8;
  //Serial.println("Time after third shift-insert:");
  //Serial.println(next_feeding_time_persistent);
  //  next_feeding_time_persistent += (long)prep_for_eeprom[3] ;
  //Serial.println("Time after fourth shift-insert:");
  //Serial.println(next_feeding_time_persistent);

  if(watersensorcounter % 6 == 0) {  
    if(lastgoodtime==0) requestTime();
    Serial.println("current status:");
    Serial.print("           last known time:");
    Serial.println(lastgoodtime);
    Serial.print("          time since start:");
    Serial.println(millis());
    Serial.print("        last snake feeding:");
    Serial.println(last_snake_feeding);
    Serial.print("        next snake feeding:");
    Serial.println(next_feeding_time_persistent);
    Serial.print("best guess at current time:");
    unsigned long current_time_best_guess =millis()/1000 + lastgoodtime - millis_at_lastgoodtime/1000;
    Serial.println(current_time_best_guess);
    unsigned int minutes_until_next_feeding=(next_feeding_time_persistent-current_time_best_guess)/60;
    unsigned int hours_until_next_feeding=(next_feeding_time_persistent-current_time_best_guess)/60/60;
    unsigned int seconds_until_next_feeding=next_feeding_time_persistent-current_time_best_guess;
    int days_until_next_feeding=(next_feeding_time_persistent-current_time_best_guess)/60/60/24;
    if(days_until_next_feeding < 0 || days_until_next_feeding >6) days_until_next_feeding=0;
    Serial.print("      days to next feeding:");
    Serial.println(days_until_next_feeding);
    Serial.print("     hours to next feeding:");
    Serial.println(hours_until_next_feeding);
    Serial.print("   minutes to next feeding:");
    Serial.println(minutes_until_next_feeding);
    Serial.print("   seconds to next feeding:");
    Serial.println(seconds_until_next_feeding);
    Serial.print("    next feeding in binary:");
    Serial.println(next_feeding_time_persistent,BIN);
    //clear leds
    for(int i = 0 ; i < 6; i++) {
      digitalWrite(leds[i],LOW); 
    }

    for(int i = 0; i<days_until_next_feeding; i++) {
      digitalWrite(leds[i],HIGH);
    }
    //for(int i = 0; i<4;i++) {
    //Serial.print("EEPROM ");
    //Serial.print(i);
    //Serial.print(":");
    //  Serial.println(EEPROM.read(i),BIN);
    //}
    if(set_feeding_time == true) {
      set_feeding_time=false;
      Serial.println("setting new feeding time!!!");
      unsigned long next_feeding_time=current_time_best_guess + last_snake_feeding/1000 + 604800;
      byte prep_for_eeprom[4] = {
        next_feeding_time >>24 & 0xff,
        next_feeding_time >>16 & 0xff,
        next_feeding_time >>8 & 0xff,
        next_feeding_time & 0xff            };
      Serial.print(                "first byte:");
      Serial.println(prep_for_eeprom[3],BIN);
      Serial.print(               "second byte:");
      Serial.println(prep_for_eeprom[2],BIN);
      Serial.print(                "third byte:");
      Serial.println(prep_for_eeprom[1],BIN);
      Serial.print(               "fourth byte:");
      Serial.println(prep_for_eeprom[0],BIN);
      for(int i = 0 ; i < 4 ; i++ ) {
        if(prep_for_eeprom[i] != EEPROM.read(i)) EEPROM.write(i,prep_for_eeprom[i]);
        Serial.println("WROTE TO EEPROM!!!!");
      }
      next_feeding_time_persistent=next_feeding_time;
    }
    digitalWrite(watersensorpower,HIGH);
    delay(10);
    int watersensorstate = analogRead(watersensor);
    Serial.println(watersensorstate);
    if(watersensorstate > 500) {
      digitalWrite(led,LOW);
      Serial.println("yes water");
      delay(1000);
    } 
    else {
      Serial.println("no water");
      const char *msg = "SNAKE:WATERLOW";       // this is your message to send
      digitalWrite(led,HIGH);
      for(int i = 0;i<10;i++) {
        vw_send((uint8_t *)msg, strlen(msg));
        vw_wait_tx();          // Wait for message to finish
        Serial.println("sending");
      }
      delay(1000);
    }
    digitalWrite(watersensorpower,LOW);
  } 
  else {
    delay(1000);
  }   
  if (vw_get_message(buf, &buflen)) // check to see if anything has been received
  {
    int i;
    // Message with a good checksum received.
    char temp=0;
    Serial.print("length:");
    Serial.println(buflen);
    for (i = 0; i < buflen; i++)
    {   
      temp=(char)buf[i];
      Serial.print(temp);  // the received data is stored in buffer
    }
    Serial.print(" ");
    Serial.print(counter++);
    Serial.println();
    //TR is code for "time response"  If the snakemail arduino gets this, it should receive something like TR1393802208.  TR
    //matches below, and the number is "time since the epoch".
    if(buf[0]=='T' && buf[1]=='R') {
      //recreate buf string, without the first two digits, so that it's easier to turn into a "long".
      char date[11] = {
        buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],'\0'                              };
      unsigned long reported_time = atol(date);
      if (lastgoodtime == reported_time) {
      } 
      else {
        lastgoodtime=reported_time;
        Serial.print("date is: ");
        Serial.println(lastgoodtime);
        millis_at_lastgoodtime=millis();
      }
    }
    //SR is code for "status request".  If the snakemail arduino gets this, it should then send a response that gives the server its' 
    //current best guess at the current time, and it's current number for when the snakes should be fed next.
    if(buf[0]=='S' && buf[1]=='R') {
    } 
  }
  int buttonstate=digitalRead(button);
  if(buttonstate == LOW) {
    Serial.print(buttonstate);
    Serial.println("button pressed");
    long nextfeeding = 0;//(now.unixtime() + 7 * 86400L);
    Serial.print("next feeding");
    Serial.println(nextfeeding);
    requestTime();
    last_snake_feeding=millis();
    set_feeding_time=true;
  }
}

void requestTime() {
  const char *msg = "TIME";       // this is your message to send
  for(int i = 0;i<10;i++) {
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();          // Wait for message to finish
    Serial.println("sending time request");
  }
}






