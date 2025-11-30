#pragma once

#include <chrono>
#include <iostream>

namespace butter {

/// @brief A simple timer for measuring code execution duration in milliseconds.
///
/// Usage:
/// ```cpp
/// {
///     Timer timer("MyTask");
///     // Code to measure
/// } // Timer prints duration when going out of scope
/// ```
class Timer {
  public:
    /// @brief Constructs the timer and starts timing immediately.
    /// @param t_name Name of the timer for display (default "Timer").
    Timer(const char* t_name = "Timer") : m_name{t_name} {
        if (ENABLE_TIMING) {
            m_startTimepoint = std::chrono::high_resolution_clock::now();
        }
    }

    /// @brief Destructor stops the timer and prints the elapsed time.
    ~Timer() { Stop(); }

    /// @brief Stops the timer and prints the elapsed time if timing is enabled.
    void Stop() {
#ifndef NO_TIMING
        if (!ENABLE_TIMING || m_stopped)
            return;
        m_stopped = true;

        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start =
            std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << m_name << " : " << ms << " ms\n";
#endif
    }

    /// @brief Global flag to enable or disable timing output.
    static bool ENABLE_TIMING;

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint; ///< Start time of the timer
    const char* m_name;                                                           ///< Name for display
    bool m_stopped = false;                                                       ///< Flag to prevent multiple stops
};

} // namespace butter
