#include "StateClient.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include <WiFi.h>
#include <time.h>

#if __has_include("secrets.h")
#include "secrets.h"
#endif

bool StateClient::begin()
{
    if (m_pollingTaskHandle != nullptr)
    {
        return true;
    }

    configTime(0, 0, "pool.ntp.org", "time.cloudflare.com");

    const BaseType_t result = xTaskCreate(
        pollingTask,
        "state-poll",
        12 * 1024,
        this,
        1,
        &m_pollingTaskHandle);
    if (result != pdPASS)
    {
        Serial.println("Could not create state polling task.");
        return false;
    }

    return true;
}

std::string StateClient::getState() const
{
    switch (m_lastState.load(std::memory_order_relaxed))
    {
    case State::Sleep:
        return "Sleep";
    case State::Waiting:
        return "Waiting";
    case State::Happy:
        return "Happy";
    case State::Sad:
        return "Sad";
    case State::Think:
        return "Think";
    case State::Attention:
        return "Attention";
    case State::Unknown:
        return {};
    }
    return {};
}

void StateClient::pollingTask(void *parameter)
{
    StateClient *client = static_cast<StateClient *>(parameter);
    while (time(nullptr) < 1700000000)
    {
        if (!client->m_timeSyncWarningLogged)
        {
            Serial.println("Waiting for system time synchronization before HTTPS polling.");
            client->m_timeSyncWarningLogged = true;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    while (true)
    {
        client->fetchState();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void StateClient::fetchState()
{
#if defined(WORKER_URL)
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Cannot poll state: Wi-Fi is not connected.");
        return;
    }

    String url = WORKER_URL;
    if (url.endsWith("/"))
    {
        url.remove(url.length() - 1);
    }
    url += "/state";

    NetworkClientSecure secureClient;
    secureClient.useBuiltinCACertBundle();

    HTTPClient http;
    http.setConnectTimeout(5000);
    http.setTimeout(5000);
    if (!http.begin(secureClient, url))
    {
        Serial.println("Could not initialize state request.");
        return;
    }

    const int statusCode = http.GET();
    if (statusCode != HTTP_CODE_OK)
    {
        Serial.printf("State request failed with HTTP status %d.\n", statusCode);
        http.end();
        return;
    }

    const String response = http.getString();
    http.end();

    const int valueStart = response.indexOf(':') + 2;
    const int valueEnd = response.indexOf('"', valueStart);
    const String state = response.substring(valueStart, valueEnd);

    State parsedState = State::Unknown;
    if (state == "Sleep")
    {
        parsedState = State::Sleep;
    }
    else if (state == "Waiting")
    {
        parsedState = State::Waiting;
    }
    else if (state == "Happy")
    {
        parsedState = State::Happy;
    }
    else if (state == "Sad")
    {
        parsedState = State::Sad;
    }
    else if (state == "Think")
    {
        parsedState = State::Think;
    }
    else if (state == "Attention")
    {
        parsedState = State::Attention;
    }

    if (parsedState == State::Unknown)
    {
        Serial.printf("Unknown state '%s' in response.\n", state.c_str());
        return;
    }

    m_lastState.store(parsedState, std::memory_order_relaxed);
#else
    Serial.println("WORKER_URL is not configured in include/secrets.h.");
#endif
}