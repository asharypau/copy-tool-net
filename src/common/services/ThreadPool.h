#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _tasks;

    std::mutex _mutex;
    std::condition_variable _condition;
    bool _stop;
};

#endif  // THREAD_POOL_H
