#pragma once
#include <chrono>

class FrameCounter {
public:
    FrameCounter() : m_tp(std::chrono::high_resolution_clock::now()),
        m_delta(),
        m_count(0),
        m_fps(0)
    {
    }

    auto update() -> void
    {
        auto now = std::chrono::high_resolution_clock::now();
        m_delta += (now - m_tp);
        m_tp = now;

        m_count++;

        if (m_delta > std::chrono::seconds(1)) {
            m_delta -= std::chrono::seconds(1);
            m_fps = m_count;
            m_count = 0;
        }
    }

    auto framerate() -> int
    {
        return m_fps;
    }

private:
    std::chrono::high_resolution_clock::time_point m_tp;
    std::chrono::nanoseconds m_delta;
    int m_count;
    int m_fps;
};
