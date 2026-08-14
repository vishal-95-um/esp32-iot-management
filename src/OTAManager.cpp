#include "OTAManager.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>

bool OTAManager::update(const char* firmwareURL) {
    Serial.println();
    Serial.println("OTA Started");
    
    // 1. Check if WiFi is connected
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No WiFi Connection => So Update is not possible");
        return false;
    }

    Serial.println("WiFi Connected");

    Serial.print("WiFi IP :");
    Serial.println(WiFi.localIP());

    // 2. check if firmware url is valid
    if(firmwareURL==nullptr || strlen(firmwareURL)==0) {
       Serial.println("No valid URL provided for OTA Update");
       return false;
    }

    Serial.print("Firmware URL:");
    Serial.println(firmwareURL);
    
    // 3. create https client (netoworking layer)
    WiFiClientSecure client;
    client.setInsecure();

    Serial.println("HTTPS Client Created");

    // 4. create http client (application layer)
    HTTPClient http;
    Serial.println("HTTP Client Created");
    
    // 5. begin the http communication
    if(!http.begin(client, firmwareURL)) {
        Serial.println("OTA Failed: http.begin() failed");

        http.end();
        return false;
    }

    Serial.println("http.begin() SUCCESS");

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    // 6. send GET request
    Serial.println("Sending the GET Request...");

    int responseCode = http.GET();

    Serial.printf("HTTP response code: %d\n", responseCode);

    if(responseCode != HTTP_CODE_OK) {
        Serial.println("OTA Failed: GET Failed");
        http.end();
        return false;
    }

    Serial.println("HTTP GET Successfull");

    // 7. check firmware size
    int contentLength = http.getSize();
    Serial.printf("Firmware size: %d bytes\n", contentLength);

    if(contentLength <= 0) {
        Serial.println("OTA Failed: Invalid firmware size");

        http.end();
        return false;
    }

    // 8. start firmware flash
    Serial.println("Starting OTA Partition...");

    if(!Update.begin(contentLength)) {
       Serial.printf("OTA Failed: Update.begin() Failed. Error: %d\n", Update.getError());
       
       http.end();
       return false;
    }

    // 9. Get http data stream
    WiFiClient* stream = http.getStreamPtr();

    if(stream == nullptr) {
        Serial.println("OTA Failed: HTTP Stream is NULL");

        Update.abort();
        http.end();
        return false;
    }

    Serial.println("HTTP Stream Ready");

    // 10. Write stream into flash
    Serial.println("Downloading firmware...");
    size_t written = Update.writeStream(*stream);

    Serial.printf("Firmware written: %u / %d bytes\n", (unsigned int)written, contentLength);

    // 11. check if written size is equal to firmware size
    if(written != (size_t)contentLength) {
        Serial.println("OTA Failed: Incomplete Firmware download");
        Update.abort();
        http.end();
        return false;
    }

    Serial.println("Firmware downloaded successfully");

    // 12. Finalize the update
    if(!Update.end()) {
        Serial.printf("OTA Failed: Update.end() error: %d\n", Update.getError());
        http.end();
        return false;
    }

    Serial.println("OTA Update Finalized");

    // 13. Validate the update operation
    if(!Update.isFinished()) {
        Serial.println("OTA FAILED: Update not finished");

        http.end();
        return false;
    }

    Serial.println("OTA update completed successfully");

    http.end();

    Serial.println("========== OTA SUCCESS ==========");
    Serial.println("Firmware successfully written.");

    Serial.println("Restarting ESP32...");

    delay(1000);
    ESP.restart();
    
    return true;

}