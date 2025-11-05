#pragma once
#include <functional>
#include <vector>
#include <algorithm>

namespace butter {

template<typename... Args>
class Observer {
public:
    using Callback = std::function<void(Args...)>;

    int subscribe(Callback callback) {
        int id = ++m_nextId;
        m_observers.emplace_back(id, std::move(callback));
        return id;
    }

    void unsubscribe(int id) {
        m_observers.erase(
            std::remove_if(m_observers.begin(), m_observers.end(),
                           [id](const auto& pair) { return pair.first == id; }),
            m_observers.end());
    }

    void notify(Args... args) {
        for (auto& [id, callback] : m_observers)
            callback(args...);
    }

private:
    int m_nextId = 0;
    std::vector<std::pair<int, Callback>> m_observers;
};

} // namespace butter
