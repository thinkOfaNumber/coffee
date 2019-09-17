#ifndef __ewifi_h_
#define __ewifi_h_

#include <ESP8266WiFi.h>
#include "task.h"

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

    void CheckForWifi();
    void SetupServer();
    void Listen();
    void Talk();

public:
    void Setup();
    void Run();
    void Debug() const;
};

} // namespace idb
#endif