
// RF Link using VirtualWire to receive messages
// simplex (one-way) receiver with a 315MHz RF Link Receiver module
// rx pin 23 on mega

#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
int counter = 0;
int watersensor = 2;
int led = 4;
int watersensorpower=3;
int watersensorstate=0;
int watersensorcounter=0;
void setup()
{
  Serial.begin(9600);    

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true);    // Required for RX Link Module
  vw_setup(2000);                   // Bits per sec
  vw_set_rx_pin(11);           // We will be receiving on pin 11.
  vw_set_tx_pin(9);                // pin 9 is used as the transmit data out into the TX Link module, change this to suit your needs.
  vw_rx_start();                      // Start the receiver
  pinMode(watersensorpower,OUTPUT);
  pinMode(watersensor,INPUT);
  pinMode(led,OUTPUT);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  watersensorcounter++;
  if(watersensorcounter % 6 == 0) {

    digitalWrite(watersensorpower,HIGH);
    int watersensorstate = digitalRead(watersensor);
    Serial.print(watersensorstate);
    delay(10);
    if(watersensorstate == HIGH) {
      digitalWrite(led,LOW);
      Serial.println("we have water");
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
    ;    
    for (i = 0; i < buflen; i++)
    {   
      temp=(char)buf[i];
      Serial.print(temp);  // the received data is stored in buffer
    }
    Serial.print(" ");
    Serial.print(counter++);
    Serial.println();
  }
  //Serial.println("working");
}

