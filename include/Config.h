#pragma once

#include <Arduino.h>

struct Config {

    // 1. WiFI
    int wifiRetries = 20;

    // 2. MQTT Broker
    const char* mqttBroker = "broker.hivemq.com";
    int mqttPort = 8883;
    const char* brokerUsername = "";
    const char* brokerPassword = "";
    const char* clientId = "";

    // 3. OTA
    const char* otaTopic = "device/OTA";

    // 4. Sensor Telemetry
    String sensorRootTopic = "device/sensor";
};