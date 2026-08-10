#include "NVSManager.h"

constexpr const char* WIFI_SPACE = "wifi";
constexpr const char* SSID_KEY = "ssid";
constexpr const char* PASSWORD_KEY= "password";

bool NVSManager::begin() {
    return prefs.begin(WIFI_SPACE, false);
}

bool NVSManager::saveWiFiCredentials(const String& ssid, const String& password) {
    bool ssidSaved = prefs.putString(SSID_KEY, ssid); // returns no of bytes written
    bool passwordSaved = prefs.putString(PASSWORD_KEY, password);

    if(!ssidSaved || !passwordSaved) {
        prefs.remove(SSID_KEY);
        prefs.remove(PASSWORD_KEY);
        return false;
    }

    return true;
}

bool NVSManager::loadWiFiCredentials(String& ssid, String& password) {
    ssid = prefs.getString(SSID_KEY, "");
    password = prefs.getString(PASSWORD_KEY, "");

    return !ssid.isEmpty();

}

bool NVSManager::hasWiFiCredentials()
{
    return prefs.isKey(SSID_KEY);
}

bool NVSManager::clearWiFiCredentials()
{
    bool ok1 = prefs.remove(SSID_KEY);
    bool ok2 = prefs.remove(PASSWORD_KEY);

    return ok1 || ok2;
}

void NVSManager::end()
{
    prefs.end();
}