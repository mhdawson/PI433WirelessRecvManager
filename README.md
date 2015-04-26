# Wireless Receive Manager for PI

This project receives wireless transmissions on a raspberry pi and
posts them to an mqtt broker for handling.  It allows additional
source devices to be easily integrated.


## To build:
TODO

## Currently supported devices
Generic 2262 based device (motion detector, door sensor, etc.)
Lacross 141 temperature sensor
NexxTech temperature sensor
Blueline Power monitor

## Adding a new device
TODO

## TODOs
- add more info about build dependencies
- add more comments to the code
- add more general documenation
- add 433 wireless receiver circuit for pi
- add more specific device info
- add more info about how to figure out protocol for a new device
- add info on topics used to publish messages for each device
- make topics more easily configurable
