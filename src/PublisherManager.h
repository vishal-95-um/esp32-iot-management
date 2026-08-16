#pragma once

#include <Arduino.h>
#include "MQTTManager.h"

class PublisherManager{
public:
   PublisherManager(MQTTManager& mqtt);

   bool publish(const char* key, const char* value);
   bool publish(const char* key, int value);
   bool publish(const char* key, float value);

private:
   MQTTManager& mqtt;
};