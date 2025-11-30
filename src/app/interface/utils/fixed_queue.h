#include <deque>
#include <iostream>

/// @brief A fixed-size queue that automatically discards the oldest elements
///        when the maximum size is reached.
///
/// @tparam T Type of elements stored in the queue.
template <typename T> class FixedQueue {
  public:
    /// @brief Construct a FixedQueue with a given maximum size.
    /// @param t_maxSize Maximum number of elements the queue can hold.
    FixedQueue(size_t t_maxSize) : m_maxSize(t_maxSize) {}

    /// @brief Push a new element to the back of the queue.
    ///        If the queue is full, the oldest element is removed.
    /// @param value Element to add.
    void push(const T& value) {
        if (m_data.size() == m_maxSize) {
            m_data.pop_front();
        }
        m_data.push_back(value);
    }

    /// @brief Clear all elements from the queue.
    void clear() { m_data.clear(); }

    /// @brief Get an iterator to the beginning of the queue.
    [[nodiscard]] auto begin() const { return m_data.begin(); }

    /// @brief Get an iterator to the end of the queue.
    [[nodiscard]] auto end() const { return m_data.end(); }

    /// @brief Get the current number of elements in the queue.
    /// @return Current size of the queue.
    [[nodiscard]] size_t size() const { return m_data.size(); }

  private:
    std::deque<T> m_data; ///< Container storing the queue elements.
    size_t m_maxSize;     ///< Maximum allowed size of the queue.
};