#pragma once

#include <atomic>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>
#include <string>
#include <mutex>

class StateClient
{
public:
    bool begin();
    std::string getState() const;
    std::string getUpdatedAt() const;

private:
    static void pollingTask(void *parameter);
    void fetchState();
    static std::string getStateFromJson(const std::string &json);
    static std::string getUpdatedAtFromJson(const std::string &json);

    enum class State : uint32_t
    {
        Unknown,
        Sleep,
        Waiting,
        Happy,
        Sad,
        Think,
        Attention
    };

    TaskHandle_t m_pollingTaskHandle = nullptr;
    bool m_timeSyncWarningLogged = false;
    std::atomic<State> m_lastState{State::Unknown};
    mutable std::mutex m_lastUpdatedAtMutex;
    std::string m_lastUpdatedAt;
};
