# NexStar GPS

A project to build an open source Nexstar-compatible GPS unit.

Celestron mounts, such as the CPC, come with a GPS unit built-in. However, they can go
bad after a few years of service. This project will give you instructions on how to build
your own compatible unit using off-the-shelf components and a little programming.

## Goals

1. Construct a replacement for the CPC mount, which already has a built-in GPS.

2. Construct a SkySync-compatible unit for attaching a GPS to Celestron mounts that don't
   already have a GPS, such as the CGEM.

3. Construct a replacement for the module in the Meade LX200GPS.

4. Other mounts? Such as the Orion Atlas. (Don't know if the hand controller supports GPS.)

## Parts

To build this, you will need:

* A GPS module, such as the uBlox NEO-6M. Commonly used by hoobyists for their quadcopters.
Available for about $16.

* A small Arduino, such as the Pro Mini (5V version). Available for about $3.

* A TTL serial-to-USB adapter for programming the Arduino (if it does not have an on-board
  USB port already). Available for about $3.

## Construction

Examine the three boards in your kit.

* The GPS module should have at least four pins: VCC, GND, TXD, and RXD.

* Locate the following pins on the Arduino: VCC, 5V, GND, TX, RX, and pins 2 & 3.

* The serial-to-USB adapter should have: 5V, GND, TXD, and RXD.

For ease of connection, you'll want to solder header pins onto the boards and use
some DuPont cables to run wires between them. I suggest using right-angle header pins.

Here's a basic connection diagram:

    GPS 5V ------------ 5V                  5V ----------- 5V
    GPS GND ----------- GND      Arduino    GND ---------- GND   Serial Adapter
    GPS TX ------------ PIN 2               TX ----------- RX                      USB ----------> PC
    GPS RX ------------ PIN 3               RX ----------- TX

Once the Arduino has been programmed you'll remove the serial adapter and plug the Arduino
directly into the existing connection in the CPC mount (inside the right-hand fork arm).

## Libraries

You will need to download and install the TinyGPS++ library.

Also needed is the SoftwareSerial library, which is built in to the Arduino IDE.
