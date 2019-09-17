#include <ESP8266WiFi.h>
#include "ewifi.h"
/*
 * wifi-private.h only needs to contain:
 *
#ifndef __wifi_private_h_
#define __wifi_private_h_
const char* ssid = "YOUR-SSID-HERE";
const char* password = "YOUR-PASSWORD-HERE";
#endif
 *
 */
#include "wifi-private.h"

// For all things WiFi, Web Server, etc.

using namespace idb;

eWiFi::eWiFi() : _variables() {}

void eWiFi::Setup() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.hostname("coffee");
    WiFi.begin(ssid, password);
    _state = eWiFiState::NoWiFi;
}

void eWiFi::Run() {
    switch (_state) {
        case eWiFiState::NoWiFi:
            CheckForWifi();
            break;

        case eWiFiState::Serving:
            SetupServer();
            break;

        case eWiFiState::Listening:
            Listen();
            break;

        case eWiFiState::Connected:
            Talk();
            break;

        case eWiFiState::Cleanup:
            // Close the connection
            _client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
            _state = eWiFiState::Listening;
            break;
    }
}

void eWiFi::Debug() const {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP address: ");
        Serial.print(WiFi.localIP());
        Serial.print(", State: ");
        Serial.println(static_cast<int>(_state));
    }
    else {
        Serial.println("No WiFi");
    }
}

void eWiFi::CheckForWifi() {
    if (WiFi.status() != WL_CONNECTED) return;
    _state = eWiFiState::Serving;
}

void eWiFi::SetupServer() {
    // Set web server port number to 80
    _server = new WiFiServer(80);
    _server->begin();
    _state = eWiFiState::Listening;
}

void eWiFi::Listen() {
    _client = _server->available();
    if (!_client) return;
    Serial.println("New Client.");
    _state = eWiFiState::Connected;
}

void eWiFi::Talk() {
    // this is sample code showing two I/O on the webpage
    String output5State = "off";
    String output4State = "off";
    char *headers = new char[100];

    if (!_client.connected()) {
        _state = eWiFiState::Cleanup;
        return;
    }

    while (_client.connected() && _client.available()) {
        // read headers
        _client.readBytesUntil('\n', headers, 100);
        if (headers[0] == '\n') {
            // empty line, so end of headers
        }
    }

    _client.println("HTTP/1.1 200 OK");
    _client.println("Content-type: text/html");
    _client.println("Connection: close");
    _client.println();

    // control and read I/O

    // Display the HTML web page
    _client.println("<!DOCTYPE html><html>");
    _client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    _client.println("<link rel=\"icon\" href=\"data:,\">");
    _client.println("");
    _client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    _client.println(".io { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
    _client.println("text-decoration: none; font-size: 30px; margin: 2px; }");
    _client.println(".button2 {background-color: #77878A;} .actions {margin-bottom: 40px;}");
    _client.println(".badge { display: inline-block; padding: .25em .4em; font-size: 75%; font-weight: 700; line-height: 1;");
    _client.println("text-align: center; white-space: nowrap; vertical-align: baseline; border-radius: .25rem; transition: color .15s }");
    _client.println(".badge-on { color: #fff; background-color: #4cbb17; }");
    _client.println(".badge-off { color: #fff; background-color: #6c757d; }");
    _client.println("</style>");
    _client.println("</head>");
    _client.println("<body>");
    _client.println("<h1>Espresserver</h1>");
    _client.println("<div style=\"float: left\">");
    WriteElementHtml(_variables.Element.Get(), "Element");
    WriteElementHtml(_variables.Pump.Get(), "Pump");
    WriteElementHtml(_variables.Solenoid.Get(), "Solenoid");
    _client.println("</div>");
    _client.println("<div style=\"float: right\">");
    WriteElementHtml(_variables.AtPressure.Get(), "At Pressure");
    WriteElementHtml(_variables.GroupSwitch.Get(), "Group Switch");
    WriteElementHtml(_variables.TankWater.Get(), "Tank Water");
    WriteElementHtml(_variables.BoilerWater.Get(), "Boiler Water");
    _client.println("</div>");
    _client.println("</body></html>");

    // The HTTP response ends with another blank line
    _client.println();
    _state = eWiFiState::Cleanup;
}

void eWiFi::WriteElementHtml(bool value, char *name) {
    String badge = value ? "on" : "off";
    String force = value ? "Off" : "On";
    String display = value ? "ON" : "OFF";
    _client.print("    <p><span class=\"io\">");
    _client.print(name);
    _client.print("<span class=\"badge badge-");
    _client.print(badge);
    _client.print("\">");
    _client.print(display);
    _client.println("</span></span></p>");
    _client.print("    <p class=\"actions\"><a href=\"#\">Force ");
    _client.print(force);
    _client.print("</a> | <a href=\"#\">Release</a></p>");
    _client.println("");
}