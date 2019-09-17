#ifndef __ewifi_h_
#define __ewifi_h_

#include <ESP8266WiFi.h>
#include "task.h"
#include "variables.h"
#include "Variable.cpp"

namespace idb {

enum class eWiFiState {
    NoWiFi = 0,
    Serving,
    Listening,
    Connected,
    Cleanup
};

class eWiFi : public Task {
private:
    WiFiServer *_server;
    WiFiClient _client;
    eWiFiState _state;
    String _header;
    Variables _variables;

    void CheckForWifi();
    void SetupServer();
    void Listen();
    void Talk();
    void WriteElementHtml(bool value, char *name);

public:
    eWiFi();
    void Setup();
    void Run();
    void Debug() const;
};

} // namespace idb
#endif