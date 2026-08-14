#pragma once 
 
#include "Config.h" 
 
#include "NVSManager.h" 
#include "Provisioning.h" 
#include "WiFiManager.h" 
#include "MQTTManager.h" 
#include "PublisherManager.h" 
#include "OTAManager.h" 
#include "SensorManager.h" 
 
class ESP32IoTSDK 
{ 
private: 
 
    NVSManager nvs; 
    ProvisionManager provision; 
    WiFiManager wifiManager; 
 
    MQTTManager mqtt; 
    PublisherManager publisher; 
 
    OTAManager ota; 
 
    SensorManager sensor; 
 
    static void mqttMessageReceived(void* context, const char* topic, const char* payload);
    void handleMQTTMessage(const char* topic, const char* payload);

public: 
 
    Config config; 
 
    ESP32IoTSDK(); 
 
    bool begin(); 
    void loop(); 
 
    SensorData readSensors(); 
 
    bool publish(const char* key, float value); 
    bool publish(const char* key, int value); 
    bool publish(const char* key, const char* value); 
};