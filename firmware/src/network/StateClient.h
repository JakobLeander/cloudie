#pragma once

#include <atomic>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>
#include <string>

class StateClient
{
public:
    bool begin();
    std::string getState() const;

private:
    static void pollingTask(void *parameter);
    void fetchState();

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
};
