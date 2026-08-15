# ESP32 IoT Management Library

A comprehensive, reusable Arduino library for ESP32 that simplifies IoT development with built-in WiFi management, MQTT communication, OTA updates, and sensor integration.

## Features

✨ **Core Capabilities:**
- 🌐 **WiFi Management** - Automatic connection, reconnection, and AP provisioning mode
- 🔐 **WiFi Provisioning** - Easy device setup via WiFi Access Point configuration
- 💾 **NVS Storage** - Non-volatile storage for persistent configuration and credentials
- 📡 **MQTT Communication** - Full-featured MQTT client with pub/sub support
- 📊 **Sensor Management** - Unified interface for reading multiple sensor types
- 📤 **Telemetry Publishing** - Easy publishing of sensor data to MQTT topics
- 🔄 **OTA Updates** - Over-the-air firmware updates via MQTT
- ⚙️ **Event Handling** - MQTT message reception and processing callbacks

## Requirements

### Hardware
- **ESP32** microcontroller (e.g., ESP32-DevKit, ESP32-WROOM)

### Software Dependencies
- Arduino IDE or PlatformIO
- **PubSubClient** (^2.8) - MQTT client library

### Arduino Libraries (via Library Manager)
- ESP32 Board Support (by Espressif Systems)
- PubSubClient (by Nick O'Leary)

## Installation

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    knolleary/PubSubClient @ ^2.8
```

### Arduino IDE
1. Download this library as ZIP
2. Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file

### Manual Installation
1. Copy the library folder to your Arduino libraries directory:
   - Windows: `Documents\Arduino\libraries\`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

## Quick Start

### Basic Usage

```cpp
#include <Arduino.h>
#include "ESP32IoTSDK.h"

ESP32IoTSDK sdk;

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Configure MQTT broker
    sdk.config.mqttBroker = "test.mosquitto.org";
    sdk.config.mqttPort = 1883;
    sdk.config.clientId = "ESP32_Device_01";

    // Initialize the SDK
    if (!sdk.begin()) {
        Serial.println("ERROR: Initialization failed!");
        return;
    }

    Serial.println("✓ Setup Complete!");
}

void loop() {
    sdk.loop();  // Keep the SDK running (handles MQTT, WiFi, etc.)

    // Read sensor data
    SensorData data = sdk.readSensors();

    // Publish to MQTT
    sdk.publish("sensor/temperature", data.temperature);
    sdk.publish("sensor/humidity", data.humidity);
    sdk.publish("sensor/distance", data.distance);

    delay(30000);  // Publish every 30 seconds
}
```

## Configuration

Edit the `Config.h` file or set configuration in your sketch before calling `sdk.begin()`:

### MQTT Settings
```cpp
sdk.config.mqttBroker = "broker.hivemq.com";      // MQTT broker address
sdk.config.mqttPort = 8883;                        // Port (1883 unencrypted, 8883 SSL)
sdk.config.brokerUsername = "your_username";       // MQTT username
sdk.config.brokerPassword = "your_password";       // MQTT password
sdk.config.clientId = "ESP32_Device_01";           // Unique device ID
```

### MQTT Topics
```cpp
sdk.config.otaTopic = "device/ota";                // For OTA updates
sdk.config.statusTopic = "device/status";          // Device status reports
sdk.config.telemetryTopic = "device/telemetry";    // Sensor data
sdk.config.sensorRootTopic = "device/sensor/";     // Sensor base path
```

### WiFi Settings
```cpp
sdk.config.wifiRetries = 20;                       // WiFi connection retry attempts
sdk.config.apSSID = "ESP32_Config";                // Provisioning AP SSID
sdk.config.apPassword = "ESP32_Secure_12345";      // Provisioning AP password
```

### Sensor Settings
```cpp
sdk.config.sensorPublishInterval = 30000;          // Publish interval (ms)
sdk.config.sensorReadInterval = 5000;              // Read interval (ms)
```

## API Reference

### Main SDK Class

#### `ESP32IoTSDK()`
Constructor - initializes the SDK instance.

#### `bool begin()`
Initializes all SDK components (WiFi, NVS, MQTT, Sensors, OTA).
- **Returns:** `true` if initialization successful, `false` otherwise

#### `void loop()`
Main loop function - must be called frequently in `loop()`.
- Handles WiFi connectivity
- Maintains MQTT connection
- Processes MQTT messages
- Reads sensors on schedule

#### `SensorData readSensors()`
Reads current sensor data.
- **Returns:** `SensorData` struct with temperature, humidity, distance

#### `bool publish(const char* key, float value)`
Publishes a float value to MQTT.
- **Parameters:** 
  - `key`: MQTT topic key (appended to sensor root topic)
  - `value`: Float value to publish
- **Returns:** `true` if published successfully

#### `bool publish(const char* key, int value)`
Publishes an integer value to MQTT.
- **Parameters:** 
  - `key`: MQTT topic key
  - `value`: Integer value to publish
- **Returns:** `true` if published successfully

#### `bool publish(const char* key, const char* value)`
Publishes a string value to MQTT.
- **Parameters:** 
  - `key`: MQTT topic key
  - `value`: String value to publish
- **Returns:** `true` if published successfully

### Configuration Structure

```cpp
struct Config {
    // MQTT Broker Settings
    const char* mqttBroker;
    int mqttPort;
    const char* brokerUsername;
    const char* brokerPassword;
    const char* clientId;
    
    // MQTT Topics
    const char* otaTopic;
    const char* statusTopic;
    const char* telemetryTopic;
    String sensorRootTopic;
    
    // WiFi Settings
    int wifiRetries;
    
    // Sensor Settings
    int sensorPublishInterval;
    int sensorReadInterval;
    
    // WiFi Provisioning
    const char* apSSID;
    const char* apPassword;
    
    // Debug
    int debugBaudRate;
};
```

### Sensor Data Structure

```cpp
struct SensorData {
    float temperature;     // In °C
    float humidity;        // In %
    float distance;        // In cm
};
```

## Module Overview

### WiFiManager
Handles WiFi connection, reconnection, and connectivity status.

### Provisioning
Provides setup mode with Access Point for easy WiFi configuration on first boot.

### NVSManager
Manages non-volatile storage for persisting configuration, credentials, and device state.

### MQTTManager
Full MQTT client implementation with automatic reconnection and message handling.

### PublisherManager
Simplifies publishing sensor data and telemetry to MQTT topics.

### OTAManager
Handles over-the-air firmware updates triggered via MQTT.

### SensorManager
Unified interface for reading temperature, humidity, and distance sensors.

## Examples

### Example 1: Basic Sensor Publishing
See `examples/BasicExample/BasicExample.ino` - Reads sensors and publishes to MQTT every 30 seconds.

### Example 2: Configuration
See `examples/ConfigurationExample/ConfigurationExample.ino` - Demonstrates custom configuration setup.

## Troubleshooting

### WiFi Connection Issues
- Check MQTT broker address and port
- Verify WiFi credentials in provisioning mode
- Check board support installation (Espressif32)

### MQTT Connection Fails
- Verify broker username and password
- Ensure client ID is unique
- Check broker connectivity with external tools (mosquitto_sub)
- Use public broker like test.mosquitto.org for testing

### Sensor Reading Returns Zero
- Verify sensor wiring and GPIO assignments
- Check sensor library installation
- Review sensor-specific setup in SensorManager.cpp

### OTA Updates Not Working
- Verify device is subscribed to OTA topic
- Check firmware binary URL format
- Ensure sufficient flash space for updates

## Project Structure

```
├── README.md                          # This file
├── library.json                       # PlatformIO metadata
├── platformio.ini                     # PlatformIO config
├── include/                           # Header files
│   ├── Config.h                       # Configuration structure
│   ├── ESP32IoTSDK.h                  # Main SDK class
│   ├── WiFiManager.h
│   ├── Provisioning.h
│   ├── NVSManager.h
│   ├── MQTTManager.h
│   ├── PublisherManager.h
│   ├── OTAManager.h
│   ├── SensorManager.h
│   └── README
├── src/                               # Implementation files
│   ├── ESP32IoTSDK.cpp
│   ├── WiFiManager.cpp
│   ├── Provisioning.cpp
│   ├── NVSManager.cpp
│   ├── MQTTManager.cpp
│   ├── PublisherManager.cpp
│   ├── OTAManager.cpp
│   └── SensorManager.cpp
├── examples/                          # Usage examples
│   ├── BasicExample/
│   └── ConfigurationExample/
└── test/                              # Unit tests
```

## Common Use Cases

### 1. Simple Sensor Data Logger
Connect sensors and publish readings to MQTT every X seconds for cloud logging.

### 2. Remote Device Management
Deploy multiple ESP32s with automatic WiFi provisioning, centralized configuration via MQTT.

### 3. IoT Device Network
Build mesh-like architectures where ESP32s report to a central MQTT broker.

### 4. Firmware Management
Update all devices remotely via OTA without physical access.

### 5. Home Automation Node
Integrate with systems like Home Assistant that support MQTT.

## Security Considerations

- **Change Default Credentials:** Update provisioning AP password in Config.h
- **Use SSL/TLS:** Set `mqttPort = 8883` for encrypted MQTT connections (requires SSL certificate handling)
- **Unique Client IDs:** Each device should have a unique `clientId`
- **Broker Authentication:** Always use username/password for public MQTT brokers
- **WiFi Security:** Use WPA2/WPA3 encryption in provisioning mode
- **NVS Encryption:** Consider enabling NVS encryption for sensitive stored data

## Performance Tips

- Adjust sensor read intervals based on your application needs
- Batch multiple sensor readings before publishing to reduce MQTT messages
- Use appropriate QoS levels in MQTT configuration
- Monitor heap memory usage for long-running devices

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This library is licensed under the MIT License. See the LICENSE file for details.

## Support

For issues, feature requests, or questions:
- Open an issue on [GitHub](https://github.com/vishal-95-um/esp32-iot-management)
- Check existing examples and documentation
- Review the sensor-specific datasheets for hardware configuration

## Version History

### v0.1.0 (Current)
- Initial release
- WiFi management and provisioning
- MQTT communication
- Sensor management
- OTA updates
- NVS storage

## Author

**Vishal** - [GitHub Profile](https://github.com/vishal-95-um)

---

**Happy IoT Development! 🚀**
