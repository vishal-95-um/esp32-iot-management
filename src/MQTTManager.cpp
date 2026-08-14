#include "MQTTManager.h" 
 
MQTTManager::MQTTManager() 
   :mqttClient(wifiClient), callback(nullptr), callbackContext(nullptr) 
{ 
} 
 
bool MQTTManager::isConnected() { 
    return mqttClient.connected(); 
} 
 
bool MQTTManager::begin(const char* broker, uint16_t port) { 
    wifiClient.setInsecure(); 
 
    mqttClient.setClient(wifiClient); 
    mqttClient.setServer(broker, port); 
 
    return true; 
} 
 
bool MQTTManager::connect(const char* clientId, const char* username, const char* password) { 
    if(isConnected()) { 
        return true; 
    } 
 
    return mqttClient.connect(clientId, username, password); 
} 
 
bool MQTTManager::publish(const char* topic, const char* payload) { 
    if(!isConnected()) { 
        return false; 
    } 
 
    return mqttClient.publish(topic, payload); 
} 
 
bool MQTTManager::subscribe(const char* topic) { 
    if(!isConnected()) { 
        return false; 
    } 
 
    return mqttClient.subscribe(topic); 
} 
 
void MQTTManager::loop() { 
    mqttClient.loop(); 
} 
 
void MQTTManager::disconnect() { 
    mqttClient.disconnect(); 
} 
 
void MQTTManager::setCallback(MQTTCallback callback, void* context) { 
    this->callback = callback; 
    this->callbackContext = context; 
 
    mqttClient.setCallback( 
        [this](char* topic, byte* payload, unsigned int length) 
        { 
            char message[length + 1]; 
 
            memcpy(message, payload, length); 
            message[length] = '\0'; 
 
            if (this->callback) 
            { 
                this->callback(this->callbackContext, topic, message); 
            } 
        } 
    ); 
}