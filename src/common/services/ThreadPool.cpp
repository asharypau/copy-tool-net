#include "ThreadPool.h"

#include <cstddef>

ThreadPool::ThreadPool(size_t num_threads)
    : _stop(false)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        _threads.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(_mutex);

                        _condition.wait(
                            lock,
                            [this]
                            {
                                return _stop || !_tasks.empty();
                            });

                        if (_stop && _tasks.empty())
                        {
                            return;
                        }

                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }

                    task();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stop = true;
    }

    _condition.notify_all();

    for (std::thread& thread : _threads)
    {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _tasks.emplace(std::move(task));
    }

    _condition.notify_one();
}
