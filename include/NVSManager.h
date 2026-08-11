#pragma once

#include <Arduino.h>
#include <Preferences.h>

class NVSManager
{
public:
   bool begin();

   bool saveWiFiCredentials(const String& ssid, const String& password);
   bool loadWiFiCredentials(String& ssid, String& pasword);

   bool hasWiFiCredentials();

   bool clearWiFiCredentials();

   void end();
private:
   Preferences prefs;
};