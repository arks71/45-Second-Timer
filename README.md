# 45-Second-Timer
A 45 second timer for roller hockey rinks based on the Sparkfun Large Digit Driver &amp; Arduino

For a standalone version - use the RollerHockey45SecondTimerAnalog.ino script

For a remotely operated version - use the RollerHockey45SecondTimerTransmitter.ino on one Arduino with digital buttons connected to pins 2 and 3, or an analog button on pin A0,  and the RollerHockey45SecondTimerReceiver.ino on the other Arduino (or multiple Arduinos for multiple receivers) connected to the large digit drivers.  Both Arduinos need connecting to a wireless transceiver, e.g. the nrf24L01 based chips.

The latest update of the receiver supports using a Micro:bit plugged directly into the receiver Arduino so that the Micro:bit can send serial commands to the Arduino.  This way the Arduino can be controlled via a set of micro:bits as remote controls running my other 'wearable timer' project.
