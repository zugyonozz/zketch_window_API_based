#pragma once
#include <windows.h>

namespace z {

enum class TimerMode {
    Simple,     // Hanya Sleep
    Precise     // Sleep + busy-wait
};

class Timer {
public:
    Timer(TimerMode mode = TimerMode::Simple)
        : m_mode(mode) {
        QueryPerformanceFrequency(&m_freq);
        start();
    }

    void start() {
        QueryPerformanceCounter(&m_startTime);
        m_prevTime = m_startTime;
        m_currTime = m_startTime;
        m_deltaTime = 0.0f;
    }

    void reset() {
        start();
    }

    void tick() {
        QueryPerformanceCounter(&m_currTime);
        m_deltaTime = static_cast<float>(m_currTime.QuadPart - m_prevTime.QuadPart) / m_freq.QuadPart;
        m_prevTime = m_currTime;
    }

    float deltaTime() const {
        return m_deltaTime;
    }

    float totalTime() const {
        return static_cast<float>(m_currTime.QuadPart - m_startTime.QuadPart) / m_freq.QuadPart;
    }

    void delayMilliseconds(int ms) {
        Sleep(ms);
    }

    void sleepToFps(float targetFps) {
        float targetFrameTime = 1.0f / targetFps;
        float waitTime = targetFrameTime - m_deltaTime;

        if (waitTime <= 0.0f) return;

        if (m_mode == TimerMode::Simple) {
            Sleep(static_cast<DWORD>(waitTime * 1000));
        } else {
            // Hybrid: sleep sebagian, busy-wait sisanya
            if (waitTime > 0.002f)
                Sleep(static_cast<DWORD>((waitTime - 0.001f) * 1000));

            LARGE_INTEGER start, now;
            QueryPerformanceCounter(&start);
            float elapsed = 0.0f;

            do {
                QueryPerformanceCounter(&now);
                elapsed = static_cast<float>(now.QuadPart - start.QuadPart) / m_freq.QuadPart;
            } while (elapsed < waitTime);
        }
    }

private:
    TimerMode m_mode;

    LARGE_INTEGER m_freq;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_prevTime;
    LARGE_INTEGER m_currTime;

    float m_deltaTime = 0.0f;
};

}