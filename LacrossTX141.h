// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _LACROSSTX141_DEVICE
#define _LACROSSTX141_DEVICE

#include "Device.h"

#define BITS_IN_MESSAGE 32

class LacrossTX141 : public Device {
   public:
      LacrossTX141();

      virtual int deviceType(void);
      virtual char* deviceName(void);

      virtual void processPulse(long duration);
      virtual void decodeMessage(Message* message);
      virtual void publishTopic(Message* message, char* buffer, int maxLength);

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
