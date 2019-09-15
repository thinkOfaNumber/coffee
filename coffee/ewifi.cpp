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

using namespace idb;

void eWiFi::Setup() {
    WiFi.hostname("coffee");
    WiFi.begin(ssid, password);
}

const char *eWiFi::GetSsid() const {
    return ssid;
}

void eWiFi::PrintLog() const {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("No WiFi");
    }
}