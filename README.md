# Wireless Receive Manager for PI

This project receives wireless transmissions on a raspberry pi and
posts them to an mqtt broker for handling.  It allows additional
source devices to be easily integrated.


## To build:
TODO

## Currently supported devices
- Generic 2262 based device (motion detector, door sensor, etc.)
- Lacross 141 temperature sensor
- NexxTech temperature sensor
- Blueline Power monitor

## Adding a new device
TODO

## Reciver

The receiver I used is an RXB6:

![picture receiver 1](pictures/receiver_1.jpg?raw=yes)
![picture receiver 2](pictures/receiver_2.jpg?raw=yes)

It works well from the basement to the second floor in my house and is available on ebay for about $3 Canadian: [RXB6](http://www.ebay.ca/itm/Super-heterodyne-OOK-Wireless-Receiver-Module-Strong-Interference-433MHZ-116dBm-/271638472090?pt=LH_DefaultDomain_0&hash=item3f3eea259a).  While this one works well for me, any other 433 receiver should work with this project as well.



## TODOs
- add more info about build dependencies
- add more comments to the code
- add more general documenation
- add 433 wireless receiver circuit for pi
- add more specific device info
- add more info about how to figure out protocol for a new device
- add info on topics used to publish messages for each device
- make topics more easily configurable

