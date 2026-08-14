#include "ESP32IoTSDK.h"

ESP32IoTSDK::ESP32IoTSDK()
    : provision(nvs),
      wifiManager(nvs, provision),
      publisher(mqtt)
{
}

bool ESP32IoTSDK::begin()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("       ESP32 IoT SDK");
    Serial.println("==============================");

    if (!nvs.begin())
    {
        Serial.println("NVS Initialization Failed");
        return false;
    }

    if (!wifiManager.connect())
    {
        Serial.println("WiFi Initialization Failed");
        return false;
    }

    Serial.println("WiFi Connected");

    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    sensor.begin();

    mqtt.begin(
        config.mqttBroker,
        config.mqttPort
    );

    if (!mqtt.connect(
            config.clientId,
            config.brokerUsername,
            config.brokerPassword))
    {
        Serial.println("MQTT Connection Failed");
        return false;
    }

    mqtt.setCallback(
        ESP32IoTSDK::mqttMessageReceived,
        this
    );

    mqtt.subscribe(config.otaTopic);

    Serial.println("MQTT Connected");

    Serial.print("Subscribed to: ");
    Serial.println(config.otaTopic);

    Serial.println("==============================");
    Serial.println("       SDK Started");
    Serial.println("==============================");

    return true;
}

void ESP32IoTSDK::loop()
{
    mqtt.loop();

    provision.handleClient();
}

SensorData ESP32IoTSDK::readSensors()
{
    return sensor.read();
}

bool ESP32IoTSDK::publish(const char* key, float value)
{
    return publisher.publish(key, value);
}

bool ESP32IoTSDK::publish(const char* key, int value)
{
    return publisher.publish(key, value);
}

bool ESP32IoTSDK::publish(const char* key, const char* value)
{
    return publisher.publish(key, value);
}

void ESP32IoTSDK::mqttMessageReceived(
    void* context,
    const char* topic,
    const char* payload)
{
    ESP32IoTSDK* sdk = static_cast<ESP32IoTSDK*>(context);

    sdk->handleMQTTMessage(topic, payload);
}

void ESP32IoTSDK::handleMQTTMessage(
    const char* topic,
    const char* payload)
{
    Serial.println();
    Serial.println("===== MQTT MESSAGE =====");

    Serial.print("Topic: ");
    Serial.println(topic);

    Serial.print("Payload: [");
    Serial.print(payload);
    Serial.println("]");

    if (strcmp(topic, config.otaTopic) == 0)
    {
        Serial.println("OTA command received");

        String url = String(payload);
        url.trim();

        Serial.print("OTA URL: [");
        Serial.print(url);
        Serial.println("]");

        if (url.startsWith("https://"))
        {
            Serial.println("Valid HTTPS URL");
            Serial.println("Starting OTA...");

            ota.update(url.c_str());
        }
        else
        {
            Serial.println("Invalid OTA URL");
            Serial.println("OTA cancelled");
        }
    }
}