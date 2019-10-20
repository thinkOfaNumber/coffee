#include <ESP8266WiFi.h>
#include <string.h>
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
            ServeEventClients();
            _client = _server->available();
            if (!_client) return;
            Route(_client);
            break;
    }
}

void eWiFi::Debug() const {
    // todo move from debug loop to ServeEventClients with a timer/counter rate limit
    for (int i = 0; i < nClients; i++) {
        _clients[i].print("data: AtPressure: ");
        _clients[i].print(_variables.AtPressure.Get());
        _clients[i].print("\r\n");

        _clients[i].print("data: GroupSwitch: ");
        _clients[i].print(_variables.GroupSwitch.Get());
        _clients[i].print("\r\n");

        _clients[i].print("data: TankWater: ");
        _clients[i].print(_variables.TankWater.Get());
        _clients[i].print("\r\n");
        
        _clients[i].print("data: BoilerWater: ");
        _clients[i].print(_variables.BoilerWater.Get());
        _clients[i].print("\r\n");

        _clients[i].print("data: Pump: ");
        _clients[i].print(_variables.Pump.Get());
        _clients[i].print("\r\n");

        _clients[i].print("data: Solenoid: ");
        _clients[i].print(_variables.Solenoid.Get());
        _clients[i].print("\r\n");

        _clients[i].print("data: Element: ");
        _clients[i].print(_variables.Element.Get());
        _clients[i].print("\r\n");

        _clients[i].print("\r\n");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP address: ");
        Serial.print(WiFi.localIP());
        // Serial.print(", State: ");
        // Serial.println(static_cast<int>(_state));
        Serial.println();
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

void eWiFi::Route(WiFiClient client) {
    ClientLog(client, "New Client.");
    bool success = ReadHeaders(client);
    _path[MaxHeaderSize-1] = '\0'; // just in case
    if (!success) {
        ClientLog(client, "Header failure.");
        _client.stop();
    }
    else if (strcmp((const char *)_path, "/") == 0) {
        ServeHtml(_client);
        _client.stop();
    }
    else if (strcmp((const char *)_path, "/events.sse") == 0) {
        if (nClients >= 2) {
            ClientLog(client, "429 Too Many Requests.");
            ServeError(_client, "429 Too Many Requests");
            _client.stop();
        }
        else {
            ServeEventInit(_client);
            _clients[nClients++] = _client;
        }
    }
    else if (strlen((const char *)_path) == 0) {
        // not sure why this is happening... let's try again next time around.
    }
    else {
        Serial.print("'");
        Serial.print(_path);
        Serial.println("' not found.");
        ClientLog(client, "404 Not Found.");
        ServeError(_client, "404 Not Found");
        _client.stop();
    }
}

bool eWiFi::ReadHeaders(WiFiClient client) {
    char *header = new char[MaxHeaderSize+1];
    _path[0] = '\0';

    if (!client.connected()) {
        delete[] header;
        return false;
    }

    while (client.connected() && client.available()) {
        // read headers. Usually \r\n (RFC2616)
        int nBytes = client.readBytesUntil('\r', header, MaxHeaderSize);
        int byte = client.peek();
        if (byte == 10) {
            // in case the header is not \r\n
            client.read();
        }
        header[nBytes] = '\0'; // null terminate it so string manip works
        if (nBytes > 0) {
            ClientLog(client, header);
        }
        if (nBytes == MaxHeaderSize) {
            // contains no newline - header too large
            ClientLog(client, "413 Request Entity Too Large");
            ServeError(client, "413 Request Entity Too Large");
            delete[] header;
            return false;
        }
        
        if (strncmp(header, "GET ", 4) == 0) {
            char *endPath = strstr(header, " HTTP");
            int endIndex = endPath == NULL ? 0 : (endPath - header);
            strncpy(_path, header+4, endIndex-4);
        }
    }
    delete[] header;
    return true;
}

void eWiFi::ServeHtml(WiFiClient client) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-type: text/html\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");

    client.print("<html><body><h1>Hello World</h1></body></head>\r\n");
    // The HTTP response ends with another blank line
    client.print("\r\n");
}

void eWiFi::ServeError(WiFiClient client, const char * error) {
    client.print("HTTP/1.1 ");
    client.print(error);
    client.print("\r\n");
    client.print("Content-type: text/html\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");

    client.print("<html><body><h1>");
    client.print(error);
    client.print("</h1></body></head>\r\n");
    client.print("\r\n");
}

void eWiFi::ServeEventInit(WiFiClient client) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-type: text/event-stream\r\n");
    client.print("Cache-Control: no-cache\r\n");
    client.print("Connection: keep-alive\r\n");
    client.print("\r\n");
}

void eWiFi::ServeEventClients() {
    // nothing for now
}

void eWiFi::ClientLog(WiFiClient client, const char * msg) {
    Serial.print(client.remoteIP());
    Serial.print(": '");
    Serial.print(msg);
    Serial.print("'");
    Serial.println();
}