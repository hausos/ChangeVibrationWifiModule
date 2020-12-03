# Changing vibration pattern over WiFi

## Info

The shematic uses a ESP8266 Node MCU microcontroller that has an integrated wifi module ESP-12E module.

Microcontroller ESP8266 NodeMCU is a chinese implementation of an Arduino microcontoller, which allows for an easy integration with WiFi.
The ESP-12E module uses Tensilica Xtensa® 32-bit LX106 RISC microprocessor. 

It has also 128 KB RAM and 4MB flash memory.

ESP8266 uses the programming language Lua. However in the scope of this project I use the already written library, designed for use in the Arduino environment. It can be easily added with #include <ESP8266WiFi.h>.

## Components used:
 - LED lučka
 - NodeMCU
 - OLED displaya - 1.3" OLED IIC LCD- 64x128 (prikaže kakšen je duty cycle signala, kakšen je signal in ali je wifi priključen)
 - Rotacijski enkoder (preko njega spreminjamo duty cyle signala, kar se pokaže kot sprememba intenzitete)
 - Tipka (preko nje spreminjamo nastavitve med različnimi signali (konstanten signal, sinus, žaga in kvadratni signal)

## Wiring / Schematic
For the real aplication I would need a DC motor with a weight excentric to the shaft of the motor. And you would also need a MOSFET, that would switch the motor ON and OFF.
The signal that you that would use to regulate a DC is here simulated with an LED, which allows for a better visual representation.

This is how the wiring looks like. For the presentation I used the application Fritzing.


![wiring schematic](/wiring.png)


## 