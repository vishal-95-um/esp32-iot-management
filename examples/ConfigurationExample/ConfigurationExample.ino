#include <Arduino.h>
#include "ESP32IoTSDK.h"

ESP32IoTSDK sdk;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\nESP32 IoT SDK - Configuration Example\n");

    // Configure MQTT Broker (HiveMQ Cloud example)
    sdk.config.mqttBroker = "your-broker.s1.eu.hivemq.cloud";
    sdk.config.mqttPort = 8883;
    sdk.config.brokerUsername = "your_username";
    sdk.config.brokerPassword = "your_password";

    // Configure Device
    sdk.config.clientId = "MyESP32_Device";
    sdk.config.sensorPublishInterval = 60000;  // Every 60 seconds

    // Configure Topics
    sdk.config.otaTopic = "mydevice/ota";
    sdk.config.sensorRootTopic = "mydevice/sensors/";

    // Configure WiFi Provisioning
    sdk.config.apSSID = "MyDevice_Setup";
    sdk.config.apPassword = "MySecurePass123";

    if (!sdk.begin()) {
        Serial.println("ERROR: Initialization failed!");
        return;
    }

    Serial.println("✓ Setup Complete!");
    Serial.print("Device: ");
    Serial.println(sdk.config.clientId);
    Serial.print("Broker: ");
    Serial.println(sdk.config.mqttBroker);
}

void loop() {
    sdk.loop();

    SensorData data = sdk.readSensors();
    sdk.publish("temperature", data.temperature);
    sdk.publish("humidity", data.humidity);
    sdk.publish("distance", data.distance);

    delay(sdk.config.sensorPublishInterval);
}
