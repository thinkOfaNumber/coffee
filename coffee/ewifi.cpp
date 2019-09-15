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

void eWiFi::Setup() {
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

const char *eWiFi::GetSsid() const {
    return ssid;
}

void eWiFi::PrintLog() const {
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
    // CSS to style the on/off buttons
    // Feel free to change the background-color and font-size attributes to fit your preferences
    _client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    _client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
    _client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    _client.println(".button2 {background-color: #77878A;}</style></head>");

    // Web Page Heading
    _client.println("<body><h1>ESP8266 Web Server</h1>");

    // Display current state, and ON/OFF buttons for GPIO 5
    _client.println("<p>GPIO 5 - State " + output5State + "</p>");
    // If the output5State is off, it displays the ON button
    if (output5State=="off") {
        _client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
    } else {
        _client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
    }

    // Display current state, and ON/OFF buttons for GPIO 4
    _client.println("<p>GPIO 4 - State " + output4State + "</p>");
    // If the output4State is off, it displays the ON button
    if (output4State=="off") {
        _client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
    } else {
        _client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
    }
    _client.println("</body></html>");

    // The HTTP response ends with another blank line
    _client.println();
    _state = eWiFiState::Cleanup;
}