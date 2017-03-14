# NexStar GPS

A project to build an open source Nexstar-compatible GPS unit.

Celestron mounts, such as the CPC, come with a GPS unit built-in. However, they can go
bad after a few years of service. This project will give you instructions on how to build
your own compatible unit using off-the-shelf components and a little programming.

An eventual goal is to build a unit compatble with the SkySync, which allows you to
attach a GPS to mounts that don't normally have one, such as the CGEM.

## Parts

To build this, you will need:

* A GPS module, such as the uBlox NEO-6M. Commonly used by hoobyists for their quadcopters.
Available for about $16.

* A small Arduino, such as the Pro Mini (5V version). Available for about $3.

* A TTL serial-to-USB adapter for programming the Arduino (if it does not have an on-board
  USB port already). Available for about $3.
