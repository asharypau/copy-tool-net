#include "WorkersPool.h"

#include "../../utils/Logger.h"

using namespace Server;

WorkersPool::WorkersPool(boost::asio::io_context& context)
    : _mtx(),
      _cv(),
      _timeout(5000),
      _max_pool_size(std::thread::hardware_concurrency() - 2),  // - 2 (the main + _manager)
      _curr_pool_size(0),
      _pool(),
      _manager(&WorkersPool::manage, this),
      _manager_tasks(),
      _context(context)
{
}

WorkersPool::~WorkersPool()
{
    ManagerTask manager_task;
    manager_task.required_pool_size = 0;
    manager_task.stop_signal = true;
    manager_task.clear_signal = true;

    {
        std::unique_lock<std::mutex> lock(_mtx);
        _manager_tasks.push(manager_task);
    }

    _cv.notify_one();

    if (_manager.joinable())
    {
        _manager.join();
    }
}

void WorkersPool::resize(const std::size_t sessions_count)
{
    std::size_t required_pool_size = get_required_pool_size(sessions_count);
    std::size_t curr_pool_size = 0;
    {
        std::unique_lock<std::mutex> lock(_mtx);
        curr_pool_size = _curr_pool_size;  // make a copy to avoid data race
    }

    if (curr_pool_size == required_pool_size)
    {
        return;
    }

    ManagerTask manager_task;
    manager_task.required_pool_size = required_pool_size;

    if (curr_pool_size > required_pool_size)
    {
        manager_task.clear_signal = true;

        std::unique_lock<std::mutex> lock(_mtx);
        _manager_tasks.push(manager_task);
    }
    else if (curr_pool_size < required_pool_size)
    {
        manager_task.extend_signal = true;

        std::unique_lock<std::mutex> lock(_mtx);
        _manager_tasks.push(manager_task);
    }

    _cv.notify_one();
}

std::size_t Server::WorkersPool::get_required_pool_size(const std::size_t sessions_count)
{
    std::size_t required_workers_size = (sessions_count + sessions_per_worker - 1) / sessions_per_worker;  // (a + b - 1) / b (so that the result is rounded up)

    if (required_workers_size <= 1)  // 1 (the main)
    {
        return 0;
    }

    return required_workers_size - 1;  // - 1 (the main)
}

void WorkersPool::manage()
{
    while (true)
    {
        std::optional<ManagerTask> manager_task;
        {
            std::unique_lock<std::mutex> lock(_mtx);

            if (_manager_tasks.empty())
            {
                _cv.wait(
                    lock,
                    [this]
                    {
                        return !_manager_tasks.empty();
                    });
            }

            manager_task.emplace(_manager_tasks.front());
        }

        if (manager_task->clear_signal)
        {
            for (size_t extra_workers = _pool.size() - manager_task->required_pool_size; extra_workers > 0; --extra_workers)
            {
                _pool.back()->deactivate();
                _pool.pop_back();
            }
        }
        else if (manager_task->extend_signal && _pool.size() < _max_pool_size)
        {
            for (size_t extra_workers = manager_task->required_pool_size - _pool.size(); extra_workers > 0; --extra_workers)
            {
                std::unique_ptr<Worker> worker = std::make_unique<Worker>(
                    [this]
                    {
                        _context.run_one_for(_timeout);
                    });
                _pool.emplace_back(std::move(worker));

                if (_pool.size() == _max_pool_size)
                {
                    break;
                }
            }
        }

        {
            std::unique_lock<std::mutex> lock(_mtx);
            _curr_pool_size = _pool.size();
            _manager_tasks.pop();
        }

        if (manager_task->stop_signal)
        {
            break;
        }
    }
}
