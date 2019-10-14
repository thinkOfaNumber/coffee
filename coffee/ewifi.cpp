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

        // case eWiFiState::Connected:
        //     Talk();
        //     break;

        // case eWiFiState::Cleanup:
        //     // Close the connection
        //     _client.stop();
        //     Serial.println("Client disconnected.");
        //     Serial.println("");
        //     _state = eWiFiState::Listening;
        //     break;
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

void eWiFi::Route(WiFiClient client) {
    ClientLog(client, "New Client.");
    bool success = ReadHeaders(client);
    _path[99] = '\0'; // just in case
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
}

bool eWiFi::ReadHeaders(WiFiClient client) {
    char *header = new char[MaxHeaderSize+1];
    _path[0] = '\0';

    if (!client.connected()) {
        delete[] header;
        return false;
    }

    while (client.connected() && client.available()) {
        // read headers
        int nBytes = client.readBytesUntil('\n', header, MaxHeaderSize);
        header[nBytes] = '\0';
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
            strncpy(_path, header+4, MaxHeaderSize-4);
            _path = "/"; // todo
            ClientLog(client, _path);
        }
    }
    delete[] header;
    return true;
}

void eWiFi::ServeHtml(WiFiClient client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<html><body><h1>Hello World</h1></body></head>");
    // The HTTP response ends with another blank line
    client.println();
}

void eWiFi::ServeError(WiFiClient client, const char * error) {
    client.print("HTTP/1.1 ");
    client.println(error);
    client.println("Content-type: text/html");
    client.println("Connection: close");
    client.println();

    client.print("<html><body><h1>");
    client.print(error);
    client.println("</h1></body></head>");
    client.println();
}

void eWiFi::ServeEventInit(WiFiClient client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/event-stream");
    client.println("Cache-Control: no-cache");
    client.println("Connection: keep-alive");
    client.println();

    client.println("<html><body><h1>Too Many Requests</h1></body></head>");
    client.println();
}

void eWiFi::ServeEventClients() {
    // nothing for now
}

void eWiFi::ClientLog(WiFiClient client, const char * msg) {
    Serial.print(client.remoteIP());
    Serial.print(": ");
    Serial.println(msg);
}