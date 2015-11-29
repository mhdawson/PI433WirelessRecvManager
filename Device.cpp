// Copyright 2014-2015 the project authors as listed in the AUTHORS file.
// All rights reserved. Use of this source code is governed by the
// license that can be found in the LICENSE file.

#include "Device.h"

Device::Device() {
}

void Device::setQueue(MessageQueue* queue) {
   this->queue = queue;
};


int Device::numMessages(void) {
  return 1;
}

void Device::getMessageText(int messageNum, Message* message, char* buffer, int maxLength) {
   strncpy(buffer, message->text, maxLength);
}

void Device::publishTopic(int messageNum, Message* message, char* buffer, int maxLength) {
   publishTopic(message, buffer, maxLength);
}
