#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

namespace cpptest {

template <typename T>
class ConcurrentQueue {
private:
    std::queue<T> m_queue;
    std::condition_variable m_cv;
    std::mutex m_mutex;

public:

    template<typename T1>
    void Enqueue(T1&& item) {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_queue.push(std::forward<T1>(item));

        m_cv.notify_one();
    }

    T Dequeue() {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_queue.empty(); });

        T item = std::move(m_queue.front());
        m_queue.pop();

        return item;
    }

    int Count() {
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_queue.size();
    }

};

}
