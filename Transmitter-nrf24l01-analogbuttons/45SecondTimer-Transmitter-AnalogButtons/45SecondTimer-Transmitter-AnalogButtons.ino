    /*
 A 45 Second Timer for Roller Hockey Rinks (based on a project called 'Controlling large 7-segment displays' by Nathan Seidle of SparkFun Electronics
 By: Tony Sollars
 Date: March 18th 2020
 License: GNU GPLv3

 This is the transmitter version component of the timer
 
 Here's how to hook up the Arduino pins to the Large Digit Driver IN

 Arduino pin 6 -> CLK (Green on the 6-pin cable)
 5 -> LAT (Blue)
 7 -> SER on the IN side (Yellow)
 5V -> 5V (Orange)
 Power Arduino with 12V and connect to Vin -> 12V (Red)
 GND -> GND (Black)

 There are two connectors on the Large Digit Driver. 'IN' is the input side that should be connected to
 your microcontroller (the Arduino). 'OUT' is the output side that should be connected to the 'IN' of addtional
 digits.

 Each display will use about 150mA with all segments and decimal point on.

*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
const int channel=0;

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte segmentClock = 6;
byte segmentLatch = 5;
byte segmentData = 7;
int val = 0;     // variable for reading the pin status
int ledPin = LED_BUILTIN; // choose the pin for the LED
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void setup()
{
  Serial.begin(9600);
  Serial.println("Roller Hockey Timer - Transmitter");

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(A0, INPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  setUpRadio();
}

void setUpRadio()
{
  Serial.println("Setting up radio receiver.");
  radio.begin();                  //Starting the Wireless communication
  radio.setChannel(channel);
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MAX);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
  Serial.println("Radio operating on Channel ");
  Serial.println(radio.getChannel());
}

void loop()
{
  val = analogRead(A0);  // read input value
  if ( (val>=0 && val <20) || (val>30 && val<60))
  {
    const char text[] = "START";
    Serial.println("Analog Button start");
    radio.write(&text, sizeof(text));
    Serial.println(val);
    //Sending the message to receiver
  }
  else if ((val>60 && val<600))  // (buttons C-70 to 110 (64 in reality on my test kit), d-110 to 150, e 150 to 600)
  {
    const char text[] = "STOP";
    Serial.println("Analog Button stop");
    Serial.println(val);
    radio.write(&text, sizeof(text));                  //Sending the message to receiver
  } 
  else
  {
    digitalWrite(ledPin, HIGH);
  }
}
