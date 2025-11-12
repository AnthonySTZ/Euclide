#include <deque>
#include <iostream>

template<typename T>
class FixedQueue {

public:
    FixedQueue(size_t t_maxSize) : m_maxSize(t_maxSize) {}

    void push(const T& value) {
        if (m_data.size() == m_maxSize) {
            m_data.pop_front();
        }
        m_data.push_back(value);
    }
    void clear() {
        m_data.clear();
    }

    auto begin() { return m_data.begin(); }
    auto end() { return m_data.end(); }
    size_t size() const { return m_data.size(); }

private:
    std::deque<T> m_data;
    size_t m_maxSize;
};