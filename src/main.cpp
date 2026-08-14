#include <Arduino.h>
#include "ESP32IoTSDK.h"

ESP32IoTSDK sdk;

void setup() {
    Serial.begin(115200);
    delay(1000);

    sdk.config.mqttBroker = "broker.hivemq.com";
    sdk.config.mqttPort = 8883;
    sdk.config.brokerUsername = "your_username";
    sdk.config.brokerPassword = "your_password";
    sdk.config.clientId = "ESP32_Device";
    sdk.config.otaTopic = "device/ota";
    sdk.config.sensorRootTopic = "device/sensor/";
    sdk.config.sensorPublishInterval = 3000;
    sdk.config.debugBaudRate = 115200;
    sdk.config.wifiRetries = 20;
    sdk.config.apSSID = "ESP32_Config";
    sdk.config.apPassword = "ESP32_Secure_12345";

    Serial.println("\nESP32 IoT Device Starting...");
    if (!sdk.begin()) {
        Serial.println("ERROR: SDK initialization failed!");
        return;
    }
    Serial.println("Setup Complete!\n");
}

void loop() {
    sdk.loop();

    SensorData data = sdk.readSensors();
    sdk.publish("temperature", data.temperature);
    sdk.publish("humidity", data.humidity);
    sdk.publish("distance", data.distance);

    delay(sdk.config.sensorPublishInterval);

}
