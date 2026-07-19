#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace gs {
namespace calc {

template <typename T>
class SafeQueue {
public:
    SafeQueue() = default;
    //~SafeQueue() = default;

    SafeQueue(const SafeQueue&) = delete;
    SafeQueue& operator=(const SafeQueue&) = delete;

    void Push(const T& item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(item);
        m_cv.notify_one();
    }

    void WaitAndPop(T& item) {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [this]() { return !m_queue.empty(); });

        item = std::move(m_queue.front());
        m_queue.pop();
    }

    bool TryPop(T& item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        item = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    size_t Size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
};

} // namespace calc
} // namespace gs
