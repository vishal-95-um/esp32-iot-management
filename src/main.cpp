#include <Arduino.h>

#include "NVSManager.h"
#include "Provisioning.h"
#include "WiFiManager.h"

NVSManager nvs;
ProvisionManager provisioning(nvs);
WiFiManager wifi(nvs, provisioning);

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("ESP32 Starting...");

    nvs.begin();

    wifi.connect();
}

void loop()
{
    provisioning.handleClient();

    delay(10);
}