// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _DEVICE
#define _DEVICE

#include "MessageQueue.h"
#include <string.h>

class Device {
   protected:
      MessageQueue* queue; 

   public:
      Device();
      void setQueue(MessageQueue* queue);

      // can be overriden if the device has more than one message
      virtual int numMessages(void);
      virtual void getMessageText(int messageNum, Message* message, char* buffer, int maxLength);
      virtual void publishTopic(int messageNum, Message* message, char* buffer, int maxLength);

      // must be implemented by sub class
      virtual int deviceType(void) = 0;
      virtual char* deviceName(void) = 0;
      virtual void processPulse(long duration) = 0;
      virtual void decodeMessage(Message* message) = 0;
      virtual void publishTopic(Message* message, char* buffer, int maxLength) = 0;
};

#endif
