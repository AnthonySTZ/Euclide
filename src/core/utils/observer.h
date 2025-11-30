#pragma once
#include <functional>
#include <vector>
#include <algorithm>

namespace butter {

/// @brief A simple observer pattern implementation for event notifications.
///
/// @tparam Args Parameter types that observers will receive when notified.
template <typename... Args> class Observer {
  public:
    /// @brief Type of the callback function for the observer.
    using Callback = std::function<void(Args...)>;

    /// @brief Subscribes a callback to this observer.
    ///
    /// @param callback The function to be called when notify() is triggered.
    /// @return An integer ID that can be used to unsubscribe.
    inline int subscribe(Callback callback) {
        int id = ++m_nextId;
        m_observers.emplace_back(id, std::move(callback));
        return id;
    }

    /// @brief Unsubscribes a callback using its subscription ID.
    ///
    /// @param id The ID returned by subscribe().
    inline void unsubscribe(int id) {
        m_observers.erase(
            std::remove_if(m_observers.begin(), m_observers.end(), [id](const auto& pair) { return pair.first == id; }),
            m_observers.end());
    }

    /// @brief Notifies all subscribed callbacks with the given arguments.
    ///
    /// @param args Arguments to pass to the callbacks.
    inline void notify(Args... args) {
        for (auto& [id, callback] : m_observers)
            callback(args...);
    }

  private:
    int m_nextId = 0;                                  ///< Incremental ID for each subscription.
    std::vector<std::pair<int, Callback>> m_observers; ///< List of subscribers.
};

} // namespace butter
