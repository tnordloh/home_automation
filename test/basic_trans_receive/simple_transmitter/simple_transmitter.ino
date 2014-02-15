// RF Link using VirtualWire to Transmit messages
// simplex (one-way) receiver with a 315MHz RF Link Transmitter module
// tx pin 3 on Duemilanova (arduino)

 
#include <VirtualWire.h>  // you must download and install the VirtualWire.h to your hardware/libraries folder
#undef int
#undef abs
#undef double
#undef float
#undef round
void setup()
{
  Serial.begin(9600);
     // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for RF Link module
    vw_setup(2000);                 // Bits per sec
    vw_set_tx_pin(9);                // pin 3 is used as the transmit data out into the TX Link module, change this to suit your needs.
}

void loop()
{
    const char *msg = "LMR-II Rocks";       // this is your message to send
    Serial.println("sending");
   vw_send((uint8_t *)msg, strlen(msg));
   vw_wait_tx();                                          // Wait for message to finish
   delay(200);
}
