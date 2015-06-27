// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

// This is second version of the device used to send messages to 2262 devices
// which fully decodes the tristate bits versus the old version which 
// only support H and Floating. As a consequence the message is a string
// of 0, 1 or F chars as opposed to an integer.


#ifndef _DEVICE2262N_DEVICE
#define _DEVICE2262N_DEVICE

#include "Device.h"

#define BITS_IN_MESSAGE_2262 24

class Device2262n : public Device {
   public:
      Device2262n(int pulseWidth, int pulseSlack, int minRepeats, const char* baseTopic );

      virtual int deviceType(void);
      virtual char* deviceName(void);

      virtual void processPulse(long duration);
      virtual void decodeMessage(Message* message);
      virtual void publishTopic(Message* message, char* buffer, int maxLength);

   private:
      int _pulseWidth;
      int _pulseSlack;
      int _minRepeats;
      const char* _baseTopic;
      bool syncFound;
      unsigned int bitCount;
      unsigned char tristateCode[BITS_IN_MESSAGE_2262 + 1];
      unsigned char lastMessage[BITS_IN_MESSAGE_2262];
      unsigned int durations[BITS_IN_MESSAGE_2262];
      long pulseCount;
      long repeatCount;
};

#endif
