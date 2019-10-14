#ifndef __ewifi_h_
#define __ewifi_h_

#include <ESP8266WiFi.h>
#include "task.h"
#include "variables.h"
#include "Variable.cpp"

namespace idb {

const int MaxHeaderSize = 200;

enum class eWiFiState {
    NoWiFi = 0,
    Serving,
    Listening
};

class eWiFi : public Task {
private:
    WiFiServer *_server;
    WiFiClient *_clients = new WiFiClient[2];
    WiFiClient _client;
    eWiFiState _state;
    String _header;
    Variables _variables;
    char *_path = new char[MaxHeaderSize];
    int nClients = 0;

    void CheckForWifi();
    void SetupServer();
    void ServeEventClients();
    void Route(WiFiClient client);
    bool ReadHeaders(WiFiClient client);
    void ServeHtml(WiFiClient client);
    void ServeEventInit(WiFiClient client);
    void ServeError(WiFiClient client, const char * error);
    void ClientLog(WiFiClient client, const char * msg);

public:
    eWiFi();
    void Setup();
    void Run();
    void Debug() const;
};

} // namespace idb
#endif