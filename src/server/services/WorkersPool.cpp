#include "WorkersPool.h"

using namespace Server;

WorkersPool::Worker::Worker()
    : _is_active(false),
      _worker()
{
}

WorkersPool::Worker::Worker(Worker&& other)
    : _is_active(other._is_active),
      _worker(std::move(other._worker))
{
    other.deactivate();
    other._worker.reset();
}

WorkersPool::Worker::~Worker()
{
    if (_worker && _worker->joinable())
    {
        _worker->join();
    }
}

WorkersPool::Worker& WorkersPool::Worker::operator=(Worker&& other)
{
    if (this == &other)
    {
        return *this;
    }

    _is_active = other._is_active;
    _worker = std::move(other._worker);

    other.deactivate();
    other._worker.reset();

    return *this;
}

WorkersPool::WorkersPool(boost::asio::io_context& context)
    : _max_concurrency(std::thread::hardware_concurrency()),
      _workers(),
      _context(context)
{
}

void WorkersPool::resize(std::size_t sessions_count)
{
    std::size_t actual_workers_count = _workers.size() + 1;                                                 // + 1 (the main worker)
    std::size_t required_workers_count = (sessions_count + sessions_per_worker - 1) / sessions_per_worker;  // (a + b - 1) / b (so that the result is rounded up)

    if ((actual_workers_count == 1 && required_workers_count == 0) || actual_workers_count == required_workers_count)
    {
        return;
    }

    if (actual_workers_count > required_workers_count)
    {
        for (size_t extra_workers = actual_workers_count - required_workers_count; extra_workers > 0; --extra_workers)
        {
            _workers.back().deactivate();
            _workers.pop_back();
        }
    }
    else if (actual_workers_count < required_workers_count)
    {
        _workers.emplace_back();
        _workers.back().activate(
            [this]
            {
                _context.run_one();
            });
    }
}