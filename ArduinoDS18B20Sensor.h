// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _ARDUINO_DS18B20_SENSOR_DEVICE
#define _ARDUINO_DS18B20_SENSOR_DEVICE

#include "Device.h"

#define BITS_IN_MESSAGE 32

class ArduinoDS18B20Sensor : public Device {
   public:
      ArduinoDS18B20Sensor();

      virtual int deviceType(void);
      virtual char* deviceName(void);

      virtual void processPulse(long duration);
      virtual void decodeMessage(Message* message);
      virtual int numMessages(void);
      virtual void getMessageText(int messageNum, Message* message, char* buffer, int maxLength);
      virtual void publishTopic(Message* message, char* buffer, int maxLength);
      virtual void publishTopic(int messageNum, Message* message, char* buffer, int maxLength);

   private:
      bool syncFound;
      unsigned int bitCount;
      unsigned long code;
      long syncCount;
      unsigned int durations[BITS_IN_MESSAGE];
      long houseCode;
      long pulseCount;
      bool validateChecksum(int code);
};

#endif
