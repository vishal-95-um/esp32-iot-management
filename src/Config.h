#pragma once

#include <Arduino.h>

// ============================================================================
// ESP32 IoT SDK - Configuration File
// ============================================================================
// EDIT THIS FILE to customize your device settings.
// No need to touch the SDK code - just change the values below.

struct Config {

    // MQTT
    const char* mqttBroker = "test.mosquitto.org";
    int mqttPort = 1883;

    const char* brokerUsername = "";
    const char* brokerPassword = "";

    const char* clientId = "ESP32_Device_01";

    // Topics
    const char* otaTopic = "device/ota";
    const char* statusTopic = "device/status";
    const char* telemetryTopic = "device/telemetry";
    String sensorRootTopic = "device/sensor/";

    // WiFi
    int wifiRetries = 20;

    // Sensors
    int sensorPublishInterval = 30000;
    int sensorReadInterval = 5000;

    // Debug
    int debugBaudRate = 115200;

    // Provisioning
    const char* apSSID = "ESP32_Config";
    const char* apPassword = "ESP32_Secure_12345";
};

// ============================================================================
// QUICK SETUP EXAMPLES
// ============================================================================
//
// 📌 EXAMPLE 1: Using HiveMQ Cloud (Free MQTT Broker)
// ────────────────────────────────────────────────────
//   const char* mqttBroker = "your-cluster.s1.eu.hivemq.cloud";
//   int mqttPort = 8883;
//   const char* brokerUsername = "vishal_951";
//   const char* brokerPassword = "YourHiveMQPassword123";
//
// 📌 EXAMPLE 2: Using Local Mosquitto Broker
// ────────────────────────────────────────────────────
//   const char* mqttBroker = "192.168.1.100";  // Your computer/Raspberry Pi IP
//   int mqttPort = 1883;
//   const char* brokerUsername = "mosquitto_user";
//   const char* brokerPassword = "mosquitto_password";
//
// 📌 EXAMPLE 3: Test with Public Broker (No auth)
// ────────────────────────────────────────────────────
//   const char* mqttBroker = "test.mosquitto.org";
//   int mqttPort = 1883;
//   const char* brokerUsername = "";
//   const char* brokerPassword = "";
//
// ============================================================================