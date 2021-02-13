#include <IRremote.h>
int RECV_PIN = 12; // Infrared receiving pin
IRrecv irrecv(RECV_PIN); // Create a class object used to receive class
decode_results results; // Create a decoding results class object
void setup()
{
Serial.begin(9600); // Initialize the serial port and set the baud rate to 9600
irrecv.enableIRIn(); // Start the receiver
}

void loop() {
if (irrecv.decode(&results)) { // Waiting for decoding
Serial.println(results.value, HEX); // Print out the decoded results
handleControl(results.value);
irrecv.resume(); // Receive the next value
}
delay(100);
}

void handleControl(unsigned long value) {
delay(100);
  switch(value) {
    //Change mode
    case 0xFF30CF:
      Serial.println(0);
      break;
    //Change brightness
    case 0xFF02FD:
      Serial.println(1);
      break;
    case 0xFF9867:
      Serial.println(2);
      break;
    
  }
  
}
