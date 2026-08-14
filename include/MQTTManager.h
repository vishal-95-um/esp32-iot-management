#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class MQTTManager{
public:
   MQTTManager();

   bool begin(const char* broker, uint16_t port);
   bool connect(const char* clientId, const char* username, const char* password);

   bool publish(const char* topic, const char* payload);
   bool subscribe(const char* topic);

   void loop();
   void disconnect();

   bool isConnected();

   typedef void (*MQTTCallback)(void* context, const char* topic, const char* payload);
   void setCallback(MQTTCallback callback, void* context);

private:
   WiFiClientSecure wifiClient;
   PubSubClient mqttClient;
   MQTTCallback callback;
   void* callbackContext;
};