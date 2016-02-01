// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#ifndef _MESSAGE_QUEUE
#define _MESSAGE_QUEUE

#include <stdint.h>

#define MAX_MESSAGES 16
#define MAX_MESSAGE_TEXT_LENGTH 64

typedef struct Message {
   void* device;
   int type;
   long timestamp;
   unsigned long code;
   float value;
   uint64_t longCode;
   char text[MAX_MESSAGE_TEXT_LENGTH];
   Message* next;
} Message;

class MessageQueue {
   private:
      Message messages[MAX_MESSAGES];
      Message* newMessages;
      Message* freeMessages;

   public:
      MessageQueue(void);
      Message* getFreeMessage(void);
      void enqueueMessage(Message* message);
      Message* dequeueMessages(void);
      void returnMessages(Message* messages, Message* lastMessage);
};

#endif
