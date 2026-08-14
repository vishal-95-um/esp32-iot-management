#include <Arduino.h>

#include "NVSManager.h"
#include "Provisioning.h"
#include "WiFiManager.h"

#include "MQTTManager.h"
#include "PublisherManager.h"

#include "OTAManager.h"

#include "SensorManager.h"

NVSManager nvs;
ProvisionManager provision(nvs);
WiFiManager wifiManager(nvs, provision);
 
MQTTManager mqtt;
PublisherManager publisher(mqtt);

OTAManager ota;

SensorManager sensor;

SensorData data;

void mqttMessageReceived(const char* topic, const char* payload)
{
    Serial.println();
    Serial.println("===== MQTT MESSAGE =====");

    Serial.print("Topic: ");
    Serial.println(topic);

    Serial.print("Payload: [");
    Serial.print(payload);
    Serial.println("]");


    if (strcmp(topic, "device/ota") == 0)
    {
        Serial.println("OTA command received");

        String url = String(payload);

        // Remove \r, \n and spaces
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

void setup() {
    Serial.begin(115200);
    delay(1000);

    if(!nvs.begin()) {
        Serial.println("NVS Initialization Failed");
        return;
    }

    if(!wifiManager.connect()){
        Serial.println("WiFi Initialization Failed");
        return;
    }

    Serial.println("WiFi Connected");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    sensor.begin();

    mqtt.begin("b1887e3d84b04a238960f0804352d949.s1.eu.hivemq.cloud", 8883);

    if(!mqtt.connect("ESP32_Client", "vishal_951", "12345678")){
        Serial.println("MQTT Connection Failed");
        return;
    }

    mqtt.setCallback(mqttMessageReceived);

    mqtt.subscribe("device/ota");

    Serial.println("MQTT ESP32_Client Connected");
    Serial.println("Subscribed to: device/ota");

}

void loop() {
    mqtt.loop();

    provision.handleClient();

    data = sensor.read();

    publisher.publish("temeprature", data.temperature);
    publisher.publish("humidity", data.humidity);
    publisher.publish("distance", data.distance);

    delay(10000);
}