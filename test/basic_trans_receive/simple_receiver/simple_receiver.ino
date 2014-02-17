
// RF Link using VirtualWire to receive messages
// simplex (one-way) receiver with a 315MHz RF Link Receiver module
// rx pin 23 on mega

#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round
   int counter = 0;
void setup()
{
    Serial.begin(9600);    

// Initialise the IO and ISR
    vw_set_ptt_inverted(true);    // Required for RX Link Module
    vw_setup(2000);                   // Bits per sec
    vw_set_rx_pin(11);           // We will be receiving on pin 23 (Mega) ie the RX pin from the module connects to this pin.
    vw_rx_start();                      // Start the receiver
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    //Serial.println("receiving");
 
    if (vw_get_message(buf, &buflen)) // check to see if anything has been received
    {
    int i;
     // Message with a good checksum received.
    char temp=0;;    
    for (i = 0; i < buflen; i++)
    {   temp=(char)buf[i];
        Serial.print(temp);  // the received data is stored in buffer
        }
        Serial.print(" ");
        Serial.print(counter++);
    Serial.println("");
     }
     //Serial.println("working");
}
