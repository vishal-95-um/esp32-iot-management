#include "Provisioning.h"
#include "Config.h"

extern Config config;

ProvisionManager::ProvisionManager(NVSManager& nvs)
    : nvs(nvs), server(80)
{
}

void ProvisionManager::begin()
{
    // Start ESP32 Access Point
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        config.apSSID,
        config.apPassword
    );

    IPAddress apIP = WiFi.softAPIP();

    Serial.println();
    Serial.println("================================");
    Serial.println("ESP32 WiFi Provisioning");
    Serial.println("================================");

    Serial.print("AP SSID: ");
    Serial.println(config.apSSID);

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

<title>IoT Device WiFi Provisioning</title>

<style>

* {
    box-sizing: border-box;
}

body {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Arial, sans-serif;
    background: #ffffff;
    color: #17202a;
    margin: 0;
    padding: 24px 16px;
}

.container {
    width: 100%;
    max-width: 520px;
    margin: 0 auto;
}

.header {
    text-align: center;
    margin-bottom: 26px;
}

.logo {
    width: 58px;
    height: 58px;
    margin: 0 auto 14px;
    border-radius: 16px;
    background: #111827;
    position: relative;
    box-shadow: 0 8px 20px rgba(17, 24, 39, 0.14);
}

.logo::before,
.logo::after {
    content: "";
    position: absolute;
    border: 2px solid #ffffff;
    border-radius: 50%;
    left: 50%;
    transform: translateX(-50%);
}

.logo::before {
    width: 22px;
    height: 22px;
    top: 15px;
    border-bottom-color: transparent;
    border-left-color: transparent;
    border-right-color: transparent;
    transform: translateX(-50%) rotate(180deg);
}

.logo::after {
    width: 36px;
    height: 36px;
    top: 8px;
    border-bottom-color: transparent;
    border-left-color: transparent;
    border-right-color: transparent;
    transform: translateX(-50%) rotate(180deg);
}

.logo span {
    position: absolute;
    width: 7px;
    height: 7px;
    background: #ffffff;
    border-radius: 50%;
    left: 50%;
    bottom: 11px;
    transform: translateX(-50%);
}

h1 {
    font-size: 25px;
    margin: 0 0 8px;
    letter-spacing: -0.4px;
}

.subtitle {
    color: #667085;
    font-size: 14px;
    line-height: 1.5;
    margin: 0 auto;
    max-width: 420px;
}

.steps {
    display: flex;
    align-items: center;
    margin-bottom: 24px;
}

.step {
    flex: 1;
    text-align: center;
    position: relative;
}

.step:not(:last-child)::after {
    content: "";
    position: absolute;
    height: 2px;
    background: #e5e7eb;
    top: 16px;
    left: 62%;
    width: 76%;
    z-index: 0;
}

.step.active:not(:last-child)::after,
.step.done:not(:last-child)::after {
    background: #111827;
}

.step-number {
    position: relative;
    z-index: 1;
    width: 32px;
    height: 32px;
    margin: 0 auto 7px;
    border-radius: 50%;
    border: 2px solid #d0d5dd;
    background: white;
    color: #667085;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 13px;
    font-weight: 700;
}

.step.active .step-number,
.step.done .step-number {
    background: #111827;
    color: white;
    border-color: #111827;
}

.step-label {
    font-size: 12px;
    color: #667085;
    font-weight: 600;
}

.step.active .step-label,
.step.done .step-label {
    color: #111827;
}

.card {
    border: 1px solid #e4e7ec;
    border-radius: 18px;
    padding: 22px;
    box-shadow: 0 8px 30px rgba(16, 24, 40, 0.07);
}

.step-content {
    display: none;
}

.step-content.active {
    display: block;
}

.section-title {
    font-size: 18px;
    font-weight: 700;
    margin: 0 0 6px;
}

.section-description {
    font-size: 13px;
    line-height: 1.5;
    color: #667085;
    margin: 0 0 20px;
}

.network-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    gap: 10px;
    margin-bottom: 10px;
}

.network-count {
    font-size: 12px;
    color: #667085;
}

.network-list {
    display: flex;
    flex-direction: column;
    gap: 9px;
    max-height: 270px;
    overflow-y: auto;
    padding-right: 2px;
}

.network-info {
    min-width: 0;
    display: flex;
    align-items: center;
    gap: 12px;
}

.signal {
    width: 22px;
    height: 18px;
    display: flex;
    align-items: flex-end;
    gap: 2px;
    flex: 0 0 auto;
}

.signal-bar {
    width: 4px;
    background: #d0d5dd;
    border-radius: 2px 2px 0 0;
}

.signal-bar:nth-child(1) { height: 5px; }
.signal-bar:nth-child(2) { height: 9px; }
.signal-bar:nth-child(3) { height: 13px; }
.signal-bar:nth-child(4) { height: 17px; }

.signal-bar.active {
    background: #111827;
}

.network {
    width: 100%;
    text-align: left;
    border: 1px solid #e4e7ec;
    background: white;
    border-radius: 12px;
    padding: 13px;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: space-between;
    transition: 0.15s ease;
}

.network:hover {
    border-color: #98a2b3;
}

.network.selected {
    border: 2px solid #111827;
    padding: 12px;
    background: #f9fafb;
}

.network-name {
    font-weight: 650;
    font-size: 14px;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    max-width: 75%;
}

.network-rssi {
    font-size: 12px;
    color: #667085;
    white-space: nowrap;
}

.signal {
    display: inline-block;
    margin-right: 6px;
}

.status {
    padding: 11px 13px;
    background: #f8fafc;
    border: 1px solid #eaecf0;
    border-radius: 10px;
    color: #475467;
    font-size: 13px;
    margin-bottom: 15px;
}

.input-group {
    margin-bottom: 18px;
}

label {
    display: block;
    font-size: 13px;
    font-weight: 650;
    margin-bottom: 8px;
}

.input-wrap {
    position: relative;
}

input {
    width: 100%;
    padding: 13px 44px 13px 13px;
    border: 1px solid #d0d5dd;
    border-radius: 10px;
    font-size: 14px;
    outline: none;
}

input:focus {
    border-color: #111827;
    box-shadow: 0 0 0 3px rgba(17, 24, 39, 0.08);
}

.toggle-password {
    position: absolute;
    right: 10px;
    top: 50%;
    transform: translateY(-50%);
    border: 0;
    background: transparent;
    color: #667085;
    cursor: pointer;
    padding: 6px;
}

button.primary,
button.secondary {
    width: 100%;
    padding: 13px 15px;
    border-radius: 10px;
    font-size: 14px;
    font-weight: 650;
    cursor: pointer;
}

button.primary {
    background: #111827;
    color: white;
    border: 1px solid #111827;
}

button.primary:hover {
    background: #000000;
}

button.secondary {
    background: white;
    color: #344054;
    border: 1px solid #d0d5dd;
    margin-top: 10px;
}

button:disabled {
    opacity: 0.55;
    cursor: not-allowed;
}

.navigation {
    margin-top: 18px;
}

.back-button {
    background: transparent;
    border: 0;
    color: #667085;
    font-size: 13px;
    cursor: pointer;
    padding: 7px 0;
}

.summary {
    border: 1px solid #e4e7ec;
    border-radius: 12px;
    padding: 14px;
    margin-bottom: 18px;
}

.summary-row {
    display: flex;
    justify-content: space-between;
    gap: 15px;
    padding: 7px 0;
    font-size: 13px;
}

.summary-label {
    color: #667085;
}

.summary-value {
    font-weight: 650;
    text-align: right;
    word-break: break-word;
}

.success {
    text-align: center;
    padding: 20px 5px;
}

.success-icon {
    width: 62px;
    height: 62px;
    border-radius: 50%;
    background: #111827;
    color: white;
    margin: 0 auto 15px;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 28px;
}

.small {
    font-size: 12px;
    color: #667085;
    line-height: 1.5;
}

.spinner {
    display: inline-block;
    width: 15px;
    height: 15px;
    border: 2px solid #d0d5dd;
    border-top-color: #111827;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
    vertical-align: -3px;
    margin-right: 6px;
}

@keyframes spin {
    to { transform: rotate(360deg); }
}

@media (max-width: 420px) {
    body {
        padding: 18px 12px;
    }

    .card {
        padding: 18px;
    }

    h1 {
        font-size: 22px;
    }
}

</style>
</head>

<body>

<div class="container">

    <div class="header">
        <div class="logo"><span></span></div>

        <h1>IoT Device WiFi Provisioning</h1>

        <p class="subtitle">
            Connect your IoT device to your WiFi network in just a few simple steps.
        </p>
    </div>

    <div class="steps">

        <div class="step active" id="stepIndicator1">
            <div class="step-number">1</div>
            <div class="step-label">Select WiFi</div>
        </div>

        <div class="step" id="stepIndicator2">
            <div class="step-number">2</div>
            <div class="step-label">Enter Password</div>
        </div>

        <div class="step" id="stepIndicator3">
            <div class="step-number">3</div>
            <div class="step-label">Connect</div>
        </div>

    </div>

    <div class="card">

        <!-- STEP 1 -->

        <div class="step-content active" id="step1">

            <div class="section-title">
                Choose your WiFi network
            </div>

            <p class="section-description">
                Select the network that you want this IoT device to connect to.
            </p>

            <div id="status" class="status">
                <span class="spinner"></span>
                Scanning nearby networks...
            </div>

            <div class="network-header">
                <label style="margin:0;">Available Networks</label>
                <span id="networkCount" class="network-count"></span>
            </div>

            <div id="networkList" class="network-list">
            </div>

            <button class="secondary"
                    onclick="scanNetworks()">
                Refresh Networks
            </button>

            <div class="navigation">
                <button id="nextButton"
                        class="primary"
                        onclick="goToPassword()"
                        disabled>
                    Continue
                </button>
            </div>

        </div>

        <!-- STEP 2 -->

        <div class="step-content" id="step2">

            <div class="section-title">
                Enter WiFi password
            </div>

            <p class="section-description">
                Enter the password for the selected network.
            </p>

            <div class="summary">
                <div class="summary-row">
                    <span class="summary-label">Network</span>
                    <span id="selectedSSID"
                          class="summary-value">—</span>
                </div>
            </div>

            <div class="input-group">

                <label for="password">
                    WiFi Password
                </label>

                <div class="input-wrap">

                    <input
                        id="password"
                        type="password"
                        placeholder="Enter WiFi password"
                        autocomplete="off"
                    >

                    <button
                        class="toggle-password"
                        onclick="togglePassword()"
                        type="button"
                        aria-label="Show password">
                        SHOW
                    </button>

                </div>

            </div>

            <button class="primary"
                    onclick="goToConnect()">
                Continue
            </button>

            <button class="secondary"
                    onclick="showStep(1)">
                ← Back
            </button>

        </div>

        <!-- STEP 3 -->

        <div class="step-content" id="step3">

            <div class="section-title">
                Ready to connect
            </div>

            <p class="section-description">
                Check the details below and connect your device to the selected WiFi network.
            </p>

            <div class="summary">

                <div class="summary-row">
                    <span class="summary-label">WiFi Network</span>
                    <span id="confirmSSID"
                          class="summary-value">—</span>
                </div>

                <div class="summary-row">
                    <span class="summary-label">Password</span>
                    <span class="summary-value">Protected</span>
                </div>

            </div>

            <button id="connectButton"
                    class="primary"
                    onclick="saveWiFi()">
                Connect Device
            </button>

            <button class="secondary"
                    onclick="showStep(2)">
                ← Back
            </button>

        </div>

    </div>

</div>

<script>

let selectedSSID = "";

function showStep(step)
{
    document.querySelectorAll(".step-content")
        .forEach(element =>
        {
            element.classList.remove("active");
        });

    document.getElementById("step" + step)
        .classList.add("active");

    for(let i = 1; i <= 3; i++)
    {
        const indicator =
            document.getElementById("stepIndicator" + i);

        indicator.classList.remove("active", "done");

        if(i < step)
            indicator.classList.add("done");

        if(i === step)
            indicator.classList.add("active");
    }
}

function scanNetworks()
{
    const status =
        document.getElementById("status");

    const list =
        document.getElementById("networkList");

    const nextButton =
        document.getElementById("nextButton");

    nextButton.disabled = true;

    status.innerHTML =
        '<span class="spinner"></span> Scanning nearby networks...';

    list.innerHTML = "";

    fetch("/scan")
    .then(response => response.json())
    .then(networks =>
    {
        list.innerHTML = "";

        if(networks.length === 0)
        {
            status.innerHTML =
                " No WiFi networks found";

            document.getElementById("networkCount")
                .innerText = "";

            return;
        }

        document.getElementById("networkCount")
            .innerText =
            networks.length + " network" +
            (networks.length === 1 ? "" : "s") +
            " found";

        networks.forEach(net =>
        {
            if(!net.ssid)
                return;

            const button =
                document.createElement("button");

            button.className = "network";

            button.innerHTML =
                '<span class="network-info">' +
                createSignal(net.rssi) +
                '<span class="network-name">' +
                escapeHTML(net.ssid) +
                '</span>' +
                '</span>' +
                '<span class="network-rssi">' +
                net.rssi +
                ' dBm</span>';

            button.onclick = function()
            {
                document.querySelectorAll(".network")
                    .forEach(item =>
                    {
                        item.classList.remove("selected");
                    });

                button.classList.add("selected");

                selectedSSID = net.ssid;

                nextButton.disabled = false;

                status.innerHTML =
                    " Network selected. Continue to enter the password.";
            };

            list.appendChild(button);
        });

        status.innerHTML =
            "Select your WiFi network.";

    })
    .catch(error =>
    {
        console.log(error);

        status.innerHTML =
            " Failed to scan networks. Please try again.";
    });
}

function getSignalBars(rssi)
{
    if(rssi >= -55)
        return 4;

    if(rssi >= -65)
        return 3;

    if(rssi >= -75)
        return 2;

    return 1;
}

function createSignal(rssi)
{
    const activeBars = getSignalBars(rssi);

    let html = '<span class="signal">';

    for(let i = 1; i <= 4; i++)
    {
        html +=
            '<span class="signal-bar ' +
            (i <= activeBars ? 'active' : '') +
            '"></span>';
    }

    html += '</span>';

    return html;
}

function goToPassword()
{
    if(!selectedSSID)
        return;

    document.getElementById("selectedSSID")
        .innerText = selectedSSID;

    showStep(2);

    setTimeout(function()
    {
        document.getElementById("password")
            .focus();
    }, 100);
}

function goToConnect()
{
    const password =
        document.getElementById("password").value;

    if(!password)
    {
        alert("Please enter the WiFi password.");
        return;
    }

    document.getElementById("confirmSSID")
        .innerText = selectedSSID;

    showStep(3);
}

function togglePassword()
{
    const input =
        document.getElementById("password");

    input.type =
        input.type === "password"
        ? "text"
        : "password";
}

function saveWiFi()
{
    const password =
        document.getElementById("password").value;

    const button =
        document.getElementById("connectButton");

    if(!selectedSSID)
        return;

    if(!password)
    {
        alert("Please enter the WiFi password.");
        showStep(2);
        return;
    }

    button.disabled = true;

    button.innerHTML =
        '<span class="spinner"></span> Saving credentials...';

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
            encodeURIComponent(selectedSSID) +
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

        button.disabled = false;

        button.innerText =
            "Connect Device";

        alert("Failed to save credentials. Please try again.");
    });
}

function escapeHTML(value)
{
    const div =
        document.createElement("div");

    div.innerText = value;

    return div.innerHTML;
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

        // Escape quotes and backslashes in SSID
        String ssid = WiFi.SSID(i);

        for(size_t j = 0; j < ssid.length(); j++)
        {
            char c = ssid[j];

            if(c == '"' || c == '\\')
                json += "\\";

            json += c;
        }

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

            <h2> Credentials Saved</h2>

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
            "<h2> Failed to save credentials</h2>"
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