#include "WiFiManager.h"
#include <Arduino.h>
#include <WiFi.h>

#if __has_include("secrets.h")
#include "secrets.h"
#define WIFI_CREDENTIALS_CONFIGURED
#endif

void WiFiManager::connect()
{
#ifdef WIFI_CREDENTIALS_CONFIGURED
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connecting to Wi-Fi...");
#else
    Serial.println("Wi-Fi credentials not configured; add include/secrets.h.");
#endif
}
