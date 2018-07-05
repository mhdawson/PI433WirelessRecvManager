// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include "PI433.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "BluelineDevice.h"
#include "NexxTechDevice.h"
#include "LacrossTX141.h"
#include "ArduinoTHSensor.h"
#include "ArduinoTHSensor2.h"
#include "ArduinoLightSensor.h"
#include "ArduinoDS18B20Sensor.h"
#include "Device2262.h"
#include "Device2262n.h"
#include "Device1527.h"
#include "MeatThermometer1.h"
#include "SLTX583.h"

#define NUM_REQUIRED_CERT_FILES 3 

#define MISSING_BROKER  	   -1
#define MISSING_CERT_DIR           -2 
#define INVALID_CERT_DIR           -3 
#define MISSING_CERT_FILES         -4
#define FAILED_TO_SETUP_WIRING_PI  -5
     
int main(int argc, char *argv[]) {
   int pin = 2;
   char* certsDir = NULL;

   // validate we got the required parameters 
   if (2 > argc) {
      printf("Missing mqttBroker parameter - exiting\n");
      printf("Usage: PI433Main mqtt_url <cert info dir>\n");
      return MISSING_BROKER;
   }

   if (strstr(argv[1], "ssl://") == argv[1]) {
      // ok we expect a third parameter which tells us where to look for the 
      // certificates and keys 
      // the names of the files in the directory specified must be client.cert client.key and ca.cert
      if (3 > argc) { 
         printf("Cert info dir required for connection to broker with ssl - exiting\n");
         printf("Usage: PI433Main mqtt_url <cert info dir>\n");
         return MISSING_CERT_DIR;
      }
 
      // validate the directory exists
      DIR* dirPtr = opendir(argv[2]);
      if(NULL == dirPtr) {
         printf("Cert info dir was invalid - exiting\n");
         return INVALID_CERT_DIR;
      } 
     
      struct dirent *nextEntry;
      int requiredFilesFound = 0;
      while ((nextEntry = readdir(dirPtr)) != NULL) {
         if ((strcmp(nextEntry->d_name, CA_CERT_FILE) == 0) ||
             (strcmp(nextEntry->d_name, CLIENT_CERT_FILE) == 0) ||
             (strcmp(nextEntry->d_name, CLIENT_KEY_FILE) == 0)) {
            requiredFilesFound++;
         }
      }
      closedir(dirPtr);

      if (requiredFilesFound != NUM_REQUIRED_CERT_FILES) { 
         printf("Missing cert info files, directory must contain client.key, client.cert and ca.cert\n");
         return MISSING_CERT_FILES;
      }

      certsDir = argv[2];
   }

   if(wiringPiSetup() == -1) {
      return FAILED_TO_SETUP_WIRING_PI;
   }

   PI433 receiver = PI433(pin, argv[1], certsDir);

   // setup devices, TODO make configurable
   receiver.registerDevice(new BluelineDevice(0x1efd)); //use houseCode when paper in way, reversed
   receiver.registerDevice(new NexxTechDevice());
   receiver.registerDevice(new LacrossTX141());
   receiver.registerDevice(new ArduinoTHSensor());
   receiver.registerDevice(new ArduinoTHSensor2());
   receiver.registerDevice(new ArduinoLightSensor());
   receiver.registerDevice(new ArduinoDS18B20Sensor());
   receiver.registerDevice(new Device2262());
   receiver.registerDevice(new Device2262n(200,75,2,"house/2262/200"));
   receiver.registerDevice(new Device2262n(350,50,4,"house/2262/350"));
   receiver.registerDevice(new Device1527(350,50,4,"house/1527/350"));
   receiver.registerDevice(new MeatThermometer1());
   receiver.registerDevice(new SLTX583());
   receiver.handleMessages();
}

