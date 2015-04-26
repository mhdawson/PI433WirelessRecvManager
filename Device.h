// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _DEVICE
#define _DEVICE

#include "MessageQueue.h"

class Device {
   protected:
      MessageQueue* queue; 

   public:
      Device();
      void setQueue(MessageQueue* queue);

      // must be implemented by sub class
      virtual int deviceType(void) = 0;
      virtual char* deviceName(void) = 0;
      virtual void processPulse(long duration) = 0;
      virtual void decodeMessage(Message* message) = 0;
      virtual void publishTopic(Message* message, char* buffer, int maxLength) = 0;
};

#endif
