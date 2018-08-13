# Arduino Wireless PS2 Controller Demo

Arduino wireless PS2 controller with OLED example.

# Hardware

* Cheap wireless PS2 controller from China. These are generally low quality and the buttons may or may not work.

# Connection to Arduino

* Identify the receiver pins from http://www.rhydolabz.com/wiki/?p=12663
* The PCB on mine was marked so opening the receiver is another option.
* Receiver D0 or DATA or pin 1 -> Arduino 12
* Receiver D1 or CLOCK or pin 2-> Arduino 11
* Receiver CS or ATTENTION or pin 6 -> Arduino 10
* Receiver CLK or COMMAND or pin 7 -> Arduino 9

