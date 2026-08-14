#include "PublisherManager.h"
#include "Config.h"

extern Config config;

PublisherManager::PublisherManager(MQTTManager& mqtt)
   :mqtt(mqtt)
{

}

bool PublisherManager::publish(const char* key, const char* value) {
    String topic = config.sensorRootTopic;
    topic += key;

    return mqtt.publish(topic.c_str(), value);
}

bool PublisherManager::publish(const char* key, float value) {
    char payload[16];
    snprintf(payload, sizeof(payload), "%.2f", value);

    String topic = config.sensorRootTopic;
    topic += key;

    return mqtt.publish(topic.c_str(), payload);
}

bool PublisherManager::publish(const char* key, int value) {
    char payload[16];
    snprintf(payload, sizeof(payload), "%d", value);

    String topic = config.sensorRootTopic;
    topic += key;

    return mqtt.publish(topic.c_str(), payload);
}