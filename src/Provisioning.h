#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include "NVSManager.h"

class ProvisionManager
{
public:
    ProvisionManager(NVSManager& nvs);

    void begin();
    void handleClient();

private:
    NVSManager& nvs;

    WebServer server;
    DNSServer dnsServer;

    void handleRoot();
    void handleSave();
    void handleScan();
    void handleNotFound();
};