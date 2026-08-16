#pragma once

#include <Arduino.h>

// ============================================================================
// ESP32 IoT SDK - Configuration File
// ============================================================================
// EDIT THIS FILE to customize your device settings.
// No need to touch the SDK code - just change the values below.

struct Config {

    // ========== MQTT BROKER - CHANGE THESE! ==========
    // Set your MQTT broker connection details
    
    const char* mqttBroker = "broker.hivemq.com";        // Broker address
    int mqttPort = 8883;                                 // Broker port (1883 unencrypted, 8883 SSL)
    
    // YOUR CREDENTIALS - REPLACE WITH YOUR ACTUAL VALUES
    const char* brokerUsername = "your_username";        // Change this to your MQTT username
    const char* brokerPassword = "your_password";        // Change this to your MQTT password
    
    // Example: If using HiveMQ Cloud
    // brokerPassword = "AbCdEfG123456";                // Your actual password here
    
    const char* clientId = "ESP32_Device_01";            // Unique device ID

    // ========== MQTT TOPICS - Customize for your app ==========
    const char* otaTopic = "device/ota";                 // Receives OTA update URLs
    const char* statusTopic = "device/status";           // Reports device status
    const char* telemetryTopic = "device/telemetry";     // Sends sensor data
    String sensorRootTopic = "device/sensor/";           // Base path: device/sensor/temperature, etc.

    // ========== WIFI SETTINGS ==========
    int wifiRetries = 20;                                // How many times to retry WiFi connection

    // ========== SENSOR SETTINGS ==========
    int sensorPublishInterval = 30000;                   // Publish every 30 seconds (milliseconds)
    int sensorReadInterval = 5000;                       // Read sensors every 5 seconds

    // ========== DEBUG & LOGGING ==========
    int debugBaudRate = 115200;                          // Serial monitor speed (don't change)

    // ========== WiFi PROVISIONING MODE ==========
    // When device starts without saved WiFi, it creates a setup network
    const char* apSSID = "ESP32_Config";                 // Network name for setup mode
    const char* apPassword = "ESP32_Secure_12345";       // Password to enter setup mode
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