// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _PI433
#define _PI433

#include <wiringPi.h>
#include <time.h>
#include "Device.h"

extern "C" {
    #include "MQTTClient.h"
    #include "MQTTClientPersistence.h"
}

class PI433 {
   public:
      PI433(int interrupt, char* mqttServer);
      void handleMessages();
      bool registerDevice(Device* newDevice);

   private:
      static void handleInterrupt();
      char* _mqttServer;
};

#endif
