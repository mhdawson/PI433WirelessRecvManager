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

### Lacross 141
The Lacross 141 is available at Canadian tire and often goes on sale for $10-$15
[cnd tire link](http://www.canadiantire.ca/en/pdp/la-crosse-weather-station-with-colour-frame-1427129p.html#.VV6MmlKznt8) 
![Lacross 141](pictures/Lacross-package.jpg?raw=yes)

### Generic 2262 Devices

Many different devices should work.  Currently this project only supports one protocol and I believe
there are a few different ones.  I'll look to add support for the others when I come across them.  These
are a few that I've used succesfully.  

#### Motion detector

Device: - 2262n - parameters(350,50,4,&lt;your topic&gt;)
Available from ebay, select 433 for frequency and 2262 for chipset
[ebay link motion detector](http://www.ebay.ca/itm/Wireless-Standard-PIR-Motion-Detector-Sensor-315-433-Mhz-1-5-3-3-4-7-M-/171089657359?var=&hash=item0)

![Motion Detector](pictures/433Motion.jpg)

#### Door Sensor
Device: - 2262n - parameters(350,50,4,&lt;your topic&gt;)
Available from ebay, select 433 for frequency and 2262 for chipset
[ebay link door sensor](http://www.ebay.ca/itm/Wireless-Door-Window-Entry-Detector-Sensor-Contact-315-433-Mhz-/181183039531?var=&hash=item0)
![Door sensor](pictures/433Door.jpg)


#### Smoke Detector
Device: - 2262n - parameters(350,50,4,&lt;your topic&gt;)
Available from ebay.
[ebay link smoke detector](http://www.ebay.ca/itm/321225011653?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT)

![Smoke Detector](pictures/433Smoke.jpg)

#### Remote controls for 120V switch
Device: - 2262n - parameters(200,75,2,&lt;your topic&gt;)
Available from eay.
[ebay link 120v switch](http://www.ebay.ca/itm/381117176383?_trksid=p2060353.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT)

![433Switch](pictures/433switch.jpg)

## Adding a new device
TODO

## Reciver

The receiver I used is an RXB6:

![picture receiver 1](pictures/receiver_1.jpg?raw=yes)
![picture receiver 2](pictures/receiver_2.jpg?raw=yes)

It works well from the basement to the second floor in my house and is available on ebay for about $3 Canadian: [RXB6](http://www.ebay.ca/itm/Super-heterodyne-OOK-Wireless-Receiver-Module-Strong-Interference-433MHZ-116dBm-/271638472090?pt=LH_DefaultDomain_0&hash=item3f3eea259a).  While this one works well for me, any other 433 receiver should work with this project as well.

## Circuit

The is the circuit I used to wire up the receiver to the pi:

![picture receiver circuit](pictures/receiver-circuit.jpg?raw=yes)

The resistor and diode are used to clamp the output of the tranmitter to 3.3v to
match the gpio input on the pi. 



## TODOs
- add more info about build dependencies
- add more comments to the code
- add more general documenation
- add more specific device info
- add more info about how to figure out protocol for a new device
- add info on topics used to publish messages for each device
- make topics more easily configurable

