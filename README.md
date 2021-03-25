# SEV Remote Control
Smart Electric Vehicle remote control implemented using AVR 8-bit microcontroller and Android companion app as a part of EV car conversion graduation project for Mechatronics Engineering Bachelor - Fall 2018.

## Overview
This section gives an overview of the system.

### Concept

SEV remote control is done via means of bluetooth communication between AVR controller board and any bluetooth-enabled device (Android smartphone for example).

### System Layout
System is divided into two separate modules that communicate with each other to acheive the desired output, which are:

#### SEV Controller Board Module

Controller board controls a set of relays and DAC units each connected to a specific actuator in the car.

#### SEV Companion App Module

Companion app sends commands via bluetooth to control each actuator.

## Discussion
This section discusses desicions made regarding design and implementation.

### Microcontroller

We opted for ATmega32 for SEV controller board as it has USART module and enough pins to control relays and DACs.

### Communication Protocol

We used USART protocol in order to communicate with the HC-05 bluetooth module.

### Android IDE

We used free MIT App Inventor as we lacked experience in Android app development while App Inventor offered shallow learning curve and a ton of tutorials available.

## Screenshots

[Screenshots of the companion app](Software/Companion%20App/Screenshots)

## Built With
- Atmel Studio 7 - The official AVR IDE
- AVR-gcc V5.4 - C compiler
- GNU Make - auto-generated makefile
- MIT App Inventor - Android IDE
- CadSoft EAGLE - PCB design tool

## Authors
Loay Ashraf - <loay.ashraf.96@gmail.com>

Mohamed Gamal - <muhammed.gamal051@gmail.com>

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
