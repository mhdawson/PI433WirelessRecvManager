// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include "PI433.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "BluelineDevice.h"
#include "NexxTechDevice.h"
#include "LacrossTX141.h"
#include "Device2262.h"
     
int main(int argc, char *argv[]) {
   int pin = 2;

   // validate we got the required parameters 
   if (2 > argc) {
      printf("Missing mqttBroker parameter - exiting\n");
      return -1;
   }

   if(wiringPiSetup() == -1) {
      return 0;
   }

   PI433 receiver = PI433(pin, argv[1]);

   // setup devices, TODO make configurable
   receiver.registerDevice(new BluelineDevice(0x1efd)); //use houseCode when paper in way, reversed
   receiver.registerDevice(new NexxTechDevice());
   receiver.registerDevice(new LacrossTX141());
   receiver.registerDevice(new Device2262());
   receiver.handleMessages();
}

