#include "Provisioning.h"

ProvisionManager::ProvisionManager(NVSManager& nvs)
    : nvs(nvs), server(80)
{
}

void ProvisionManager::begin()
{
    // Start ESP32 Access Point
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "ESP32_Config",
        "12345678"
    );

    IPAddress apIP = WiFi.softAPIP();

    Serial.println();
    Serial.println("================================");
    Serial.println("ESP32 WiFi Provisioning");
    Serial.println("================================");

    Serial.print("AP SSID: ");
    Serial.println("ESP32_Config");

    Serial.print("AP IP: ");
    Serial.println(apIP);

    // DNS Server

    dnsServer.start(
        53,
        "*",
        apIP
    );

    // Main configuration page

    server.on("/", HTTP_GET, [this]()
    {
        handleRoot();
    });

    server.on("/save", HTTP_POST, [this]()
    {
        handleSave();
    });

    server.on("/scan", HTTP_GET, [this]()
    {
        handleScan();
    });

    server.onNotFound([this]()
    {
        handleNotFound();
    });

    server.begin();

    Serial.println("Provisioning server started");
}

void ProvisionManager::handleRoot()
{
    String html = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>ESP32 WiFi Configuration</title>

<style>

body {
    font-family: Arial;
    background: #f4f6f8;
    margin: 0;
    padding: 20px;
}

.container {
    max-width: 450px;
    margin: auto;
    background: white;
    padding: 25px;
    border-radius: 15px;
    box-shadow: 0px 4px 15px rgba(0,0,0,0.1);
}

h2 {
    text-align: center;
}

label {
    font-weight: bold;
}

input, select, button {
    width: 100%;
    padding: 12px;
    margin-top: 8px;
    margin-bottom: 18px;
    box-sizing: border-box;
    border-radius: 8px;
    border: 1px solid #ccc;
}

button {
    background: #007bff;
    color: white;
    border: none;
    cursor: pointer;
    font-size: 16px;
}

button:hover {
    background: #0056b3;
}

#status {
    text-align: center;
    margin-bottom: 15px;
}

.refresh {
    background: #555;
}

.network {
    cursor: pointer;
}

</style>

</head>

<body>

<div class="container">

<h2>📡 IoT WiFi Configuration</h2>

<div id="status">
Scanning WiFi networks...
</div>

<label>Available Networks</label>

<select id="network">

<option value="">
Scanning...
</option>

</select>

<label>WiFi Password</label>

<input
    id="password"
    type="password"
    placeholder="Enter WiFi password"
>

<button onclick="saveWiFi()">
Connect
</button>

<button class="refresh"
        onclick="scanNetworks()">

🔄 Refresh Networks

</button>

</div>

<script>

function scanNetworks()
{
    const status =
        document.getElementById("status");

    const network =
        document.getElementById("network");

    status.innerHTML =
        "🔍 Scanning nearby networks...";

    network.innerHTML =
        "<option>Scanning...</option>";

    fetch("/scan")

    .then(response => response.json())

    .then(networks =>
    {
        network.innerHTML = "";

        if(networks.length === 0)
        {
            network.innerHTML =
                "<option>No networks found</option>";

            status.innerHTML =
                "❌ No WiFi networks found";

            return;
        }

        networks.forEach(net =>
        {
            let option =
                document.createElement("option");

            option.value = net.ssid;

            option.text =
                net.ssid +
                "  (" +
                net.rssi +
                " dBm)";

            network.appendChild(option);
        });

        status.innerHTML =
            "✅ Select your WiFi network";
    })

    .catch(error =>
    {
        console.log(error);

        status.innerHTML =
            "❌ Failed to scan networks";
    });
}


function saveWiFi()
{
    const ssid =
        document.getElementById("network").value;

    const password =
        document.getElementById("password").value;

    if(!ssid)
    {
        alert("Please select a WiFi network");
        return;
    }

    if(!password)
    {
        alert("Please enter WiFi password");
        return;
    }

    document.getElementById("status").innerHTML =
        "💾 Saving credentials...";

    fetch("/save",
    {
        method: "POST",

        headers:
        {
            "Content-Type":
            "application/x-www-form-urlencoded"
        },

        body:
            "ssid=" +
            encodeURIComponent(ssid) +
            "&password=" +
            encodeURIComponent(password)
    })

    .then(response =>
    {
        return response.text();
    })

    .then(data =>
    {
        document.body.innerHTML = data;
    })

    .catch(error =>
    {
        console.log(error);

        document.getElementById("status").innerHTML =
            "❌ Failed to save credentials";
    });
}


// Automatically scan when page loads

window.onload = function()
{
    scanNetworks();
};

</script>

</body>

</html>

)rawliteral";

    server.send(
        200,
        "text/html",
        html
    );
}


void ProvisionManager::handleScan()
{
    Serial.println("Scanning WiFi networks...");

    int count = WiFi.scanNetworks();

    String json = "[";

    for(int i = 0; i < count; i++)
    {
        if(i > 0)
            json += ",";

        json += "{";

        json += "\"ssid\":\"";
        json += WiFi.SSID(i);
        json += "\",";

        json += "\"rssi\":";
        json += WiFi.RSSI(i);

        json += "}";
    }

    json += "]";

    WiFi.scanDelete();

    server.send(
        200,
        "application/json",
        json
    );

    Serial.print("Networks found: ");
    Serial.println(count);
}


void ProvisionManager::handleSave()
{
    String ssid =
        server.arg("ssid");

    String password =
        server.arg("password");

    if(ssid.isEmpty() ||
       password.isEmpty())
    {
        server.send(
            400,
            "text/html",
            "<h2>SSID and Password can't be empty</h2>"
        );

        return;
    }

    Serial.println();
    Serial.println("Saving WiFi credentials...");
    Serial.print("SSID: ");
    Serial.println(ssid);

    if(nvs.saveWiFiCredentials(
        ssid,
        password))
    {
        server.send(
            200,
            "text/html",
            R"rawliteral(

            <html>

            <head>
            <meta name="viewport"
                  content="width=device-width">
            </head>

            <body>

            <h2>✅ Credentials Saved</h2>

            <p>
            ESP32 is restarting...
            </p>

            </body>

            </html>

            )rawliteral"
        );

        delay(1000);

        ESP.restart();
    }

    else
    {
        server.send(
            500,
            "text/html",
            "<h2>❌ Failed to save credentials</h2>"
        );
    }
}


void ProvisionManager::handleNotFound()
{
    // Redirect unknown URLs to configuration page

    server.sendHeader(
        "Location",
        "/",
        true
    );

    server.send(
        302,
        "text/plain",
        ""
    );
}


void ProvisionManager::handleClient()
{
    // Handle DNS requests
    dnsServer.processNextRequest();

    // Handle HTTP requests
    server.handleClient();
}