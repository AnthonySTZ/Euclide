#pragma once

#include <chrono>
#include <iostream>

namespace butter {

class Timer {
    
public:
    Timer(const char* t_name = "Timer") : m_name{ t_name } {
        if(s_enableTiming) {
            m_startTimepoint = std::chrono::high_resolution_clock::now();
        }
    }
    
    ~Timer() {
        Stop();
    }
    
    void Stop() {
        #ifndef NO_TIMING
        if (!s_enableTiming || m_stopped) return;
        m_stopped = true;
        
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        auto duration = end - start;
        double ms = duration * 0.001;
        
        std::cout << m_name << " : " << ms << " ms\n";
        #endif
    }

    static bool s_enableTiming;
    
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
    const char* m_name;
    bool m_stopped = false;
    
};

}