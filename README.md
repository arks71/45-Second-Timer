# 45-Second-Timer
A 45 second timer for roller hockey rinks based on the Sparkfun Large Digit Driver &amp; Arduino

There are a number of variants depending on how you want to use the timer.

1) Standard alone version using an analog button keypad
2) Standard alone version using a digital button keypad
3) Wireless version using a nrf24l01 chipset
4) Wireless version using a micro:bit for wireless comms
5) Transmitter for Wireless version #3 with digital buttons
6) Transmitter for Wireless version #3 with analog buttons

These are in the subfolders
1) 45SecondTimer-StandaloneAnalog
2) 45SecondTimer-StandaloneDigital
3) ReceiverWithNRF24L01
4) ReceiverWithMicrobit
5) Transmitter-nrf24l01-digitalbuttons
6) Transmitter-nrf24l01-analogbuttons

Corresponding circuit diagrams are now in the subfolders.

For the wireless versions - you'll either need a second Arduino with a nrf24l01 chip, or one or more micro:bits running my 'wearable' timer software

As of 6/5/2020, I've added 'resume' functionality (via a 2nd stop command) to the receivers, so that you can stop the timer for situations such as the ball going out of play and resume from the same position rather than incorrectly resetting the timer to 45 seconds.
