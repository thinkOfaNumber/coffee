#ifndef __ewifi_h_
#define __ewifi_h_

#include <ESP8266WiFi.h>

namespace idb {

enum class eWiFiState {
    NoWiFi = 0,
    Serving,
    Listening,
    Connected,
    Cleanup
};

class eWiFi {
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
    const char *GetSsid() const;
    void PrintLog() const;
};

} // namespace idb
#endif