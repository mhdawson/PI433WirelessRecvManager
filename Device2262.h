// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _DEVICE2262_DEVICE
#define _DEVICE2262_DEVICE

#include "Device.h"

#define BITS_IN_MESSAGE_2262 24

class Device2262 : public Device {
   public:
      Device2262();

      virtual int deviceType(void);
      virtual char* deviceName(void);

      virtual void processPulse(long duration);
      virtual void decodeMessage(Message* message);
      virtual void publishTopic(Message* message, char* buffer, int maxLength);

   private:
      bool syncFound;
      unsigned int bitCount;
      unsigned long code;
      unsigned int durations[BITS_IN_MESSAGE_2262];
      long pulseCount;
      long repeatCount;
      long lastMessage;
};

#endif
