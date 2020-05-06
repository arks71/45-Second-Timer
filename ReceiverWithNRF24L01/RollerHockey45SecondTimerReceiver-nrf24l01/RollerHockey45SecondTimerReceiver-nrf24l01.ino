    /*
 A 45 Second Timer for Roller Hockey Rinks (based on a project called 'Controlling large 7-segment displays' by Nathan Seidle of SparkFun Electronics
 By: Tony Sollars
 Date: March 18th 2020
 License: GNU GPLv3

 This version works with a wireless receiver so the button to operate it
 can be a distance away from the LED displays
 
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
const byte address[6] = "00001";
const int channel = 0;
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
  Serial.println("Roller Hockey Timer - Receiver with nrf24l01 wireless");
  
  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  pinMode(ledPin, OUTPUT);  // declare LED as output

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  setUpRadio();
}

void setUpRadio()
{
  Serial.println("Setting up radio receiver.");
  radio.begin();
  radio.setChannel(channel);
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MAX);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();
  Serial.println("Radio operating on Channel ");
  Serial.println(radio.getChannel());
}

double timerLength = 45000;
int number = 45;
unsigned long startTime = 0;
unsigned long currentTime = 0;
double elapsed = 0;
double remaining = 0;

const int MAX_INPUT=32;

void doStart()
{
  timerLength = 45000;
  startTime=millis();
  digitalWrite(ledPin, LOW);  // turn LED OFF
  Serial.println("Start");
  //Serial.println(startTime);
}

void doStop()
{
  if (startTime > 0) {
    currentTime = millis();
    elapsed = currentTime - startTime;
    timerLength = (timerLength - elapsed);
    startTime = 0;
    Serial.println("Stop");
  }
  else {
    // implement resume on a second 'stop' press
    startTime = millis();
    Serial.println("Resume");
  }
}

String strText="";

void loop()
{

  if (radio.available())
  {
    char text[32] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data

    strText = text;
    if (strText.startsWith("START"))
    {
      doStart();
    }
    else if (strText.startsWith("STOP"))
    {
      doStop();
    }
    else if (strText.startsWith("RESUME"))
    {
      doStop();
    }
    else if (strText!="")
    {
      Serial.println(strText);
    }
  }
    
  if (startTime!=0)
  {
    currentTime = millis();
    elapsed = currentTime - startTime;
    remaining = (timerLength-elapsed)/1000;
    //Serial.println(remaining);
    number = ceil(remaining);
    if (number<0)
    {
      number = 0;
    }
  }
  else
  {
    number = ceil(timerLength/1000);
  }

  showNumber(number);
}

//Takes a number and displays 2 numbers. Displays absolute value (no negatives)
void showNumber(float value)
{
  int number = abs(value); //Remove negative signs and any decimals

  for (byte x = 0 ; x < 2 ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
}

//Given a number, or '-', shifts it out to the display
void postNumber(byte number, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  byte segments;

  switch (number)
  {
    case 1: segments = b | c; break;
    case 2: segments = a | b | d | e | g; break;
    case 3: segments = a | b | c | d | g; break;
    case 4: segments = f | g | b | c; break;
    case 5: segments = a | f | g | c | d; break;
    case 6: segments = a | f | g | e | c | d; break;
    case 7: segments = a | b | c; break;
    case 8: segments = a | b | c | d | e | f | g; break;
    case 9: segments = a | b | c | d | f | g; break;
    case 0: segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'c': segments = g | e | d; break;
    case '-': segments = g; break;
  }

  if (decimal) segments |= dp;

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}
