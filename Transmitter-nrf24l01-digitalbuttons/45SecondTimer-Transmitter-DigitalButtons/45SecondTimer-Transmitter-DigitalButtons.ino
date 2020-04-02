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
int button_pin = 2;  // for a digital button
int stop_button_pin = 3;  // for a digital button
const int channel=0;
boolean button_state = 0;

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
  pinMode(button_pin, INPUT);
  pinMode(stop_button_pin, INPUT);
  
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
  button_state = digitalRead(button_pin);
  if(button_state == HIGH)
  {
    const char text[] = "START";
    Serial.println("Button start");
    radio.write(&text, sizeof(text));                  //Sending the message to receiver
  }

  button_state = digitalRead(stop_button_pin);
  if(button_state == HIGH)
  {
    const char text[] = "STOP";
    Serial.println("Button stop");
    radio.write(&text, sizeof(text));                  //Sending the message to receiver
  }

}
