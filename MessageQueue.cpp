// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include <string.h>
#include "MessageQueue.h"

MessageQueue::MessageQueue(void) {

   // build the list of free messages
   memset(messages, 0, MAX_MESSAGES * sizeof(Message));
   for (int i=0; i < (MAX_MESSAGES - 1) ; i++) {
      messages[i].next = &messages[i+1];
   }
   messages[MAX_MESSAGES-1].next = NULL;
   freeMessages = &(messages[0]);
   newMessages = NULL;	
}

Message* MessageQueue::getFreeMessage(void) {
   Message* newMessage = NULL; 
   do {
      newMessage = freeMessages;
      if (NULL == newMessage) {
         break;
      }
   } while (!__sync_bool_compare_and_swap(&freeMessages, newMessage, newMessage->next));
   return newMessage;
};

void MessageQueue::enqueueMessage(Message* message) {
   do {
      message->next = newMessages;
   } while (!__sync_bool_compare_and_swap(&newMessages, message->next, message));
};

Message* MessageQueue::dequeueMessages(void) {
   Message* currentMessages = NULL; 
   do {
      currentMessages = newMessages;
      if (NULL == currentMessages) {
         break;
      }
   } while (!__sync_bool_compare_and_swap(&newMessages,currentMessages, NULL));
   return currentMessages;
};

void MessageQueue::returnMessages(Message* messages, Message* lastMessage) {
   do {
      lastMessage->next = freeMessages;
   } while (!__sync_bool_compare_and_swap(&freeMessages,lastMessage->next, messages));
};

