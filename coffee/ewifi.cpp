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

eWiFi::eWiFi() : _variables() {
    for (int i = 0; i < MaxClients; i++) {
        _clients[i] = NULL;
    }
}

void eWiFi::Setup() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.hostname("coffee");
    WiFi.begin(ssid, password);
    _state = eWiFiState::NoWiFi;
}

void eWiFi::Run(unsigned int cycle) {
    switch (_state) {
        case eWiFiState::NoWiFi:
            CheckForWifi();
            break;

        case eWiFiState::Serving:
            SetupServer();
            break;

        case eWiFiState::Listening:
            ServeEventClients(cycle);
            WiFiClient client = _server->available();
            if (!client) return;
            Route(client);
            break;
    }
}

void eWiFi::Debug() const {
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

void eWiFi::Route(WiFiClient &client) {
    _path[0] = '\0';
    ClientLog(client, "New Client.");
    bool success = ReadHeaders(client);
    _path[MaxHeaderSize-1] = '\0'; // just in case
    if (!success) {
        ClientLog(client, "Header failure.");
        client.stop();
    }
    else if (strcmp((const char *)_path, "/") == 0) {
        ServeHtml(client);
        client.stop();
    }
    else if (strcmp((const char *)_path, "/events.sse") == 0) {
        if (nClients >= 2) {
            ClientLog(client, "429 Too Many Requests.");
            ServeError(client, "429 Too Many Requests");
            client.stop();
        }
        else {
            ClientLog(client, "serving events to new client");
            ServeEventInit(client);
            for (int i = 0; i < MaxClients; i++) {
                if (_clients[i] == NULL) {
                    _clients[i] = new WiFiClient(client);
                    nClients++;
                    break;
                }
            }
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
        ServeError(client, "404 Not Found");
        client.stop();
    }
}

bool eWiFi::ReadHeaders(WiFiClient &client) {
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
            _path[endIndex-4] = '\0';
            Serial.print("new path is '");
            Serial.print(_path);
            Serial.println("'");
        }
    }
    delete[] header;
    return true;
}

void eWiFi::ServeHtml(WiFiClient &client) {
    ClientLog(client, "200 OK");
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-type: text/html\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");

    client.print("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='icon' href='data:,'><style>body,textarea,input,select{background:0;border-radius:0;font:16px sans-serif;margin:0}.addon,.btn-sm,.nav,textarea,input,select{outline:0;font-size:14px}.smooth{transition:all .2s}.btn,.nav a{text-decoration:none}.container{margin:0 20px;width:auto}@media(min-width:1310px){.container{margin:auto;width:1270px}}.btn,h2{font-size:2em}h1{font-size:3em}.btn{background:#999;border-radius:6px;border:0;color:#fff;cursor:pointer;display:inline-block;margin:2px 0;padding:12px 30px 14px}.btn:hover{background:#888}.btn:active,.btn:focus{background:#777}.btn-a{background:#0ae}.btn-a:hover{background:#09d}.btn-a:active,.btn-a:focus{background:#08b}.btn-b{background:#3c5}.btn-b:hover{background:#2b4}.btn-b:active,.btn-b:focus{background:#2a4}.btn-c{background:#d33}.btn-c:hover{background:#c22}.btn-c:active,.btn-c:focus{background:#b22}.btn-sm{border-radius:4px;padding:10px 14px 11px}label>*{display:inline}form>*{display:block;margin-bottom:10px}textarea,input,select{border:1px solid #ccc;padding:8px}textarea:focus,input:focus,select:focus{border-color:#5ab}textarea,input[type=text]{-webkit-appearance:none;width:13em;outline:0}.addon{box-shadow:0 0 0 1px #ccc;padding:8px 12px}.nav,.nav .current,.nav a:hover{background:#000;color:#fff}.nav{height:24px;padding:11px 0 15px}.nav a{color:#aaa;padding-right:1em;position:relative;top:-1px}.nav .pagename{font-size:22px;top:1px}.btn.btn-close{background:#000;float:right;font-size:25px;margin:-54px 7px;display:none}@media(max-width:500px){.btn.btn-close{display:block}.nav{overflow:hidden}.pagename{margin-top:-11px}.nav:active,.nav:focus{height:auto}.nav div:before{background:#000;border-bottom:10px double;border-top:3px solid;content:'';float:right;height:4px;position:relative;right:3px;top:14px;width:20px}.nav a{display:block;padding:.5em 0;width:50%}}.row{margin:1% 0;overflow:auto}.col{float:left}.table,.c12{width:100%}.c11{width:91.66%}.c10{width:83.33%}.c9{width:75%}.c8{width:66.66%}.c7{width:58.33%}.c6{width:50%}.c5{width:41.66%}.c4{width:33.33%}.c3{width:25%}.c2{width:16.66%}.c1{width:8.33%}@media(max-width:870px){.row .col{width:100%}}</style></head><body><h1>Espresserver</h1><div class='row'> <div class='col c6'> <span class='btn btn-b btn-sm' id='Element-on' style='display: none'>Element ON</span> <span class='btn btn-c btn-sm' id='Element-off' style='display: none'>Element OFF</span> <span class='btn btn-sm' id='Element-unknown'>Element ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='Pump-on' style='display: none'>Pump ON</span> <span class='btn btn-c btn-sm' id='Pump-off' style='display: none'>Pump OFF</span> <span class='btn btn-sm' id='Pump-unknown'>Pump ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='Solenoid-on' style='display: none'>Solenoid ON</span> <span class='btn btn-c btn-sm' id='Solenoid-off' style='display: none'>Solenoid OFF</span> <span class='btn btn-sm' id='Solenoid-unknown'>Solenoid ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='AtPressure-on' style='display: none'>At Pressure ON</span> <span class='btn btn-c btn-sm' id='AtPressure-off' style='display: none'>At Pressure OFF</span> <span class='btn btn-sm' id='AtPressure-unknown'>At Pressure ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='GroupSwitch-on' style='display: none'>Group Switch ON</span> <span class='btn btn-c btn-sm' id='GroupSwitch-off' style='display: none'>Group Switch OFF</span> <span class='btn btn-sm' id='GroupSwitch-unknown'>Group Switch ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='TankWater-on' style='display: none'>Tank Water ON</span> <span class='btn btn-c btn-sm' id='TankWater-off' style='display: none'>Tank Water OFF</span> <span class='btn btn-sm' id='TankWater-unknown'>Tank Water ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> <span class='btn btn-b btn-sm' id='BoilerWater-on' style='display: none'>Boiler Water ON</span> <span class='btn btn-c btn-sm' id='BoilerWater-off' style='display: none'>Boiler Water OFF</span> <span class='btn btn-sm' id='BoilerWater-unknown'>Boiler Water ?</span> <p><a href='#'>Force On</a> | <a href='#'>Force Off</a> | <a href='#'>Release</a></p></div><div class='col c6'> </div></div><script>var app=(function (){var forceOn=function(elem, event){}; var setVal=function(key, val){/*console.log(`key ${key}; val ${val}`);*/ document.getElementById(`${key}-on`).style.display=val ? '' : 'none'; document.getElementById(`${key}-off`).style.display=val ? 'none' : ''; document.getElementById(`${key}-unknown`).style.display='none';}; var run=function(){var evtSource=new EventSource('/events.sse'); evtSource.onmessage=function(e){e.data.split('\\n').forEach(function(line){/*console.log(`line: ${line}`);*/ var keyVal=line.split(': '); setVal(keyVal[0], keyVal[1]==='1');});};}; run(); return{forceOn: forceOn};})();</script></body></html>");
    client.print("\r\n");
    // The HTTP response ends with another blank line
    client.print("\r\n");
}

void eWiFi::ServeError(WiFiClient &client, const char * error) {
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

void eWiFi::ServeEventInit(WiFiClient &client) {
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-type: text/event-stream\r\n");
    client.print("Cache-Control: no-cache\r\n");
    client.print("Connection: keep-alive\r\n");
    client.print("\r\n");
}

void eWiFi::ServeEventClients(unsigned int cycle) {
    if (cycle % 20 != 0) {
        return;
    }
    
    for (int i = 0; i < MaxClients; i++) {
        if (_clients[i] == NULL) {
            continue;
        }
        if (!_clients[i]->connected()) {
            ClientLog(*_clients[i], "disconnected");
            delete _clients[i];
            _clients[i] = NULL;
            nClients--;
            continue;
        }
    }
    
    for (int i = 0; i < MaxClients; i++) {
        if (_clients[i] == NULL) {
            continue;
        }
        Serial.print("writing data for cycle ");
        Serial.print(cycle);
        Serial.print(" to client ");
        Serial.println(i+1);
        _clients[i]->print("data: AtPressure: ");
        _clients[i]->print(_variables.AtPressure.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("data: GroupSwitch: ");
        _clients[i]->print(_variables.GroupSwitch.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("data: TankWater: ");
        _clients[i]->print(_variables.TankWater.Get());
        _clients[i]->print("\r\n");
        
        _clients[i]->print("data: BoilerWater: ");
        _clients[i]->print(_variables.BoilerWater.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("data: Pump: ");
        _clients[i]->print(_variables.Pump.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("data: Solenoid: ");
        _clients[i]->print(_variables.Solenoid.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("data: Element: ");
        _clients[i]->print(_variables.Element.Get());
        _clients[i]->print("\r\n");

        _clients[i]->print("\r\n");
    }
}

void eWiFi::ClientLog(WiFiClient &client, const char * msg) {
    Serial.print(client.remoteIP());
    Serial.print(": '");
    Serial.print(msg);
    Serial.print("'");
    Serial.println();
}