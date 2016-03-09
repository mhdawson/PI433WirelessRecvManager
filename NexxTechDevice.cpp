// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include "NexxTechDevice.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define DEVICE_ID 2
#define DEVICE_NAME "nexxtech"

#define MAX_SHORT_PULSE_LENTH 2200
#define MIN_PULSE_LENGTH 500
#define SYNC_DURATION_MIN 4000
#define SYNC_DURATION_ZERO_MAX 5000
#define EXPECTED_MESSAGE_BITS 32

NexxTechDevice::NexxTechDevice() : Device() {
   syncFound = false;
   bitCount = 0;
   code = 0;
   currentBit = 0;
   currentState = 0;
   syncCount = 0;
   shortCount = 0;
}

int NexxTechDevice::deviceType(void) {
   return DEVICE_ID;
};

char* NexxTechDevice::deviceName(void) {
   return (char*) DEVICE_NAME;
};

void NexxTechDevice::processPulse(long duration) {
   // any pulse less than MIN_PULSE_LENGTH means we have noise so we are not in the middle
   // of a transmision
   if (duration < MIN_PULSE_LENGTH) {
      syncFound = false;
      syncCount = 0;
      return;
   }

   // capture the next bit and decode if we are at the end of the transmission
   if (syncFound) {
      if (duration < MAX_SHORT_PULSE_LENTH) {
         // short pulse, we expect these in pairs
         shortCount++;
         if (shortCount == 2) {
             if (1 == currentState) {
                 code = code | currentBit;
             }
             currentBit = ((currentBit>>1) & 0x7FFFFFFF);
             shortCount = 0;
             bitCount++;
         }
      } else {
         // long pulse 
         if (shortCount > 1 )  {
            // this means we have an invalid message
            syncFound = false;
            syncCount = 0;
            return;
         }
         shortCount = 0;

         // the bit value changes for every long pulse
         currentState = (currentState + 1) %2;
         if (1 == currentState) {
            code = code | currentBit;
         }
         currentBit = ((currentBit>>1) & 0x7FFFFFFF);
         bitCount++;
      }

      // check if we have all 32 bits that we expect as part of the message
      if (EXPECTED_MESSAGE_BITS == bitCount) {
         // swap code around in place as bits are in reverse order than received
         char* bytes = (char*) &code;
         int checksum = 0;
         for (int i=0;i<8;i++) {
             char newValue = 0;
             for (int j=0;j<8;j++){
               newValue |= ((bytes[i]>>j) & 0x1 )<<(8-j-1);
             }
             bytes[i] = newValue;
             if(0 != i) {
                checksum = checksum + bytes[i];
             }
         }

         // complete the calculation of the checksum
         checksum++;
         if (0xFF < checksum) {
             checksum = (checksum + 1) & 0xFF;
         }

         // if the checksum is valid
         if((checksum == bytes[0]) && ((bytes[3] & 0xF) == 0x02)) {
            Message* newMessage = queue->getFreeMessage();
            if (NULL != newMessage) {
		memset(newMessage, 0, sizeof(Message));
                newMessage->device = (void*) this;
                newMessage->timestamp = time(NULL); 
                newMessage->code = code;
                queue->enqueueMessage(newMessage);
            } else {
               // no messages available so just drop this value
            }
         }
         syncFound = false;
      }
   } else {
      if (duration > SYNC_DURATION_MIN) {
         syncCount++;
         if (syncCount > 2) {
            if (duration < SYNC_DURATION_ZERO_MAX) {
               currentState = 0;
               // there will be two short pulses after the sync that we have to ignore
               shortCount = -2;
            } else {
               currentState = 1;
               // first bit is 1 set it in the code
               code = code | 0x80000000;

               // there will be a short pulses after the sync that we have to ignore
               shortCount = -1;
            }
            code = 0;
            syncFound = true;
            syncCount = 0;
            bitCount = 1;
            // we have already set the first bit, so start the sliding bit indicating the next
            // bit to be set to the second one.
            currentBit = 0x40000000;
         }
      } else {
         syncCount = 0;
      }
   }
};

void NexxTechDevice::decodeMessage(Message* message){
   char* bytes = (char*) &message->code;
   message->type = 1;
   message->value = ((bytes[1] & 0x0F) * 10)
                  + ((bytes[2] & 0xF0)>>4)
                  + (((float)(bytes[2] & 0x0F)) / 10);

   if (bytes[1] & 0x20) {
      message->value = message->value * -1;
   }

   sprintf(message->text,"%ld, temp: %f", message->timestamp, message->value);
};


void NexxTechDevice::publishTopic(Message* message, char* buffer, int maxLength) {
   strncpy(buffer, (char*) "house/nexxtech/temp", maxLength);
}
