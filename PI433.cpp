// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include "PI433.h"

#define MAX_TOPIC_LENGTH 1024
#define SEPARATOR "/"

typedef struct DeviceListEntry {
   Device* device;
   DeviceListEntry* next;
} DeviceListEntry;

static MessageQueue* queue;
static DeviceListEntry* devices;

PI433::PI433(int interrupt, char* mqttServer, char* certsDir) {
   wiringPiISR(interrupt, INT_EDGE_BOTH, &handleInterrupt);
   queue = new MessageQueue();
   devices = NULL;
   _mqttServer = mqttServer;
   _certsDir = certsDir;
}

// we do most of the work outside of the interrupt handler.  The interrupt
// handler allows devices to enqueue messages and then this method processes the
// messages as time permits. 
void PI433::handleMessages() {
   MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;
   Message lastHandledMessage;
   memset(&lastHandledMessage, 0, sizeof(Message));
   MQTTClient myClient = NULL;
   MQTTClient_connectOptions mqttOptions = MQTTClient_connectOptions_initializer;
   mqttOptions.keepAliveInterval = 20;

   if (strstr(_mqttServer, "ssl://") == _mqttServer) {
      // ssl is enabled so setup the options
      mqttOptions.ssl = &sslOptions;
      sslOptions.trustStore = (char*) malloc(strlen(_certsDir) + strlen(SEPARATOR) + strlen(CA_CERT_FILE) + 1);
      sslOptions.keyStore   = (char*) malloc(strlen(_certsDir) + strlen(SEPARATOR) + strlen(CLIENT_CERT_FILE) + 1);
      sslOptions.privateKey   = (char*) malloc(strlen(_certsDir) + strlen(SEPARATOR) + strlen(CLIENT_KEY_FILE) + 1);
      strcpy((char*) sslOptions.trustStore, _certsDir);
      strcpy((char*) sslOptions.keyStore, _certsDir);
      strcpy((char*) sslOptions.privateKey, _certsDir);
      strcat((char*) sslOptions.trustStore, SEPARATOR);
      strcat((char*) sslOptions.keyStore, SEPARATOR);
      strcat((char*) sslOptions.privateKey, SEPARATOR);
      strcat((char*) sslOptions.trustStore, CA_CERT_FILE);
      strcat((char*) sslOptions.keyStore, CLIENT_CERT_FILE);
      strcat((char*) sslOptions.privateKey, CLIENT_KEY_FILE);

      sslOptions.enabledCipherSuites = "TLSv1.2";
   } else {
      mqttOptions.ssl = NULL;
   }

   MQTTClient_create(&myClient, _mqttServer, "PI433RecManager", MQTTCLIENT_PERSISTENCE_NONE, NULL);
   MQTTClient_setCallbacks(myClient, NULL, NULL, NULL, NULL);

   mqttOptions.keepAliveInterval = 10;
   mqttOptions.cleansession = 1;
   mqttOptions.reliable = 0;

   int connectResult = MQTTClient_connect(myClient, &mqttOptions);
   if(MQTTCLIENT_SUCCESS != connectResult) {
      printf("Failed to connect to MQTT broker, exiting\n");
      return;
   }

   while(1) {
      char topicBuffer[MAX_TOPIC_LENGTH];
      usleep(1000 * 1000);
      MQTTClient_yield();

      // check for new messages
      Message* currentMessages = queue->dequeueMessages(); 
      if (NULL != currentMessages) { 
         Message* nextMessage = currentMessages;
         Message* lastMessage = NULL;
         while(NULL != nextMessage) {
            Device* messageDevice =  ((Device*) nextMessage->device);

            // do an post processing need to fill out the the message from the raw data
            messageDevice->decodeMessage(nextMessage);

            // check if this is a duplicate message, only handle if not a duplicate
            if (  nextMessage->timestamp > (lastHandledMessage.timestamp + 1)
                ||lastHandledMessage.value != nextMessage->value 
                ||lastHandledMessage.device != nextMessage->device
                ||lastHandledMessage.code != nextMessage->code 
                ||lastHandledMessage.type != nextMessage->type
               ) {
               // get the topic to publish the message to, a zero length string means don't publish
               topicBuffer[0] = 0;
               messageDevice->publishTopic(nextMessage, topicBuffer, MAX_TOPIC_LENGTH); 

               if (strlen(topicBuffer) != 0) {
                  // printf("%s\n", nextMessage->text);

                  int result = MQTTClient_publish(myClient, topicBuffer,
                                  strlen(nextMessage->text), nextMessage->text, 0, false, NULL); 
                  if (MQTTCLIENT_SUCCESS != result) {
                     // try to reconnect and send again
                     MQTTClient_connect(myClient, &mqttOptions);
                     result = MQTTClient_publish(myClient, topicBuffer, strlen(nextMessage->text),
                                 nextMessage->text, 0, false, NULL); 
                     if (MQTTCLIENT_SUCCESS != result) {
                        printf("publish failed:%d\n",result);
                     }
                  }
               }
               memcpy(&lastHandledMessage, nextMessage, sizeof(Message));
            }
            lastMessage = nextMessage;
            nextMessage = nextMessage->next;
         }    
         queue->returnMessages(currentMessages, lastMessage);
      }
   }
}

void PI433::handleInterrupt() {
   static unsigned int duration;
   static unsigned long lastInterruptTime;

   // get the duration of the pulse including botht the 0 and 1 state     
   long timeMicros = micros();
   duration = timeMicros - lastInterruptTime;
   lastInterruptTime = timeMicros;  

   // let each registerd device process the pulse
   DeviceListEntry* nextDevice = devices;
   while(NULL != nextDevice) {
      nextDevice->device->processPulse(duration);
      nextDevice = nextDevice->next;
   }
}

bool PI433::registerDevice(Device* newDevice) {
  
   DeviceListEntry* deviceEntry = (DeviceListEntry*) malloc(sizeof(DeviceListEntry));
   if (NULL != deviceEntry) {
      newDevice->setQueue(queue);
      deviceEntry->device = newDevice;
      deviceEntry->next = devices;
      devices = deviceEntry;
      return true;
   }
   return false;
}
