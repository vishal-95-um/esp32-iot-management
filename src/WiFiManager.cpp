#include "WiFiManager.h"
#include "NVSManager.h"
#include "Provisioning.h"
#include <WiFi.h>

WiFiManager::WiFiManager(NVSManager& nvs, ProvisionManager& provisioning)
   :nvs(nvs),
   provisioning(provisioning)
{
}

bool WiFiManager::connect() {
    if(nvs.hasWiFiCredentials()) {
        String ssid, password;
        nvs.loadWiFiCredentials(ssid, password);
        WiFi.begin(ssid.c_str(), password.c_str());

        int retries = 0;

        while(WiFi.status() != WL_CONNECTED && retries < 20){
            delay(500);
            Serial.print(".");
            retries++;
        }

        if(WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi Connected");
            return true;
        }

        Serial.println("WiFi Connection Failed");
        Serial.println("Starting the WebServer for local provisioning");
        
        provisioning.begin();
        return false;
    } else {
        Serial.println("No WiFi Credentials Found");
        Serial.println("Starting the WebServer for local provisioning");
        provisioning.begin();
        return false;
    }
}
    

