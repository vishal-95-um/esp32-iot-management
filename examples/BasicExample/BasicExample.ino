#include <Arduino.h>
#include "ESP32IoTSDK.h"

ESP32IoTSDK sdk;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\nESP32 IoT SDK - Basic Example\n");

    sdk.config.mqttBroker = "test.mosquitto.org";
    sdk.config.mqttPort = 1883;
    sdk.config.brokerUsername = "";
    sdk.config.brokerPassword = "";
    sdk.config.clientId = "ESP32_BasicExample";

    if (!sdk.begin()) {
        Serial.println("ERROR: Initialization failed!");
        return;
    }

    Serial.println("✓ Setup Complete!");
    Serial.println("Sensor data will be published to MQTT broker");
}

void loop() {
    sdk.loop();

    SensorData data = sdk.readSensors();

    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.print("°C | Humidity: ");
    Serial.print(data.humidity);
    Serial.print("% | Distance: ");
    Serial.print(data.distance);
    Serial.println("cm");

    sdk.publish("sensor/temperature", data.temperature);
    sdk.publish("sensor/humidity", data.humidity);
    sdk.publish("sensor/distance", data.distance);

    delay(30000);
}
