#include "WorkersPool.h"

#include "../../utils/Logger.h"

using namespace Server;

WorkersPool::Worker::Worker()
    : _is_active(false),
      _worker()
{
}

WorkersPool::Worker::~Worker()
{
    if (_worker && _worker->joinable())
    {
        _worker->join();
    }
}

WorkersPool::WorkersPool(boost::asio::io_context& context)
    : _max_concurrency(std::thread::hardware_concurrency()),
      _timeout(5000),
      _workers(),
      _context(context)
{
}

void WorkersPool::resize(std::size_t sessions_count)
{
    if (!_workers.empty())
    {
        std::erase_if(
            _workers,
            [](const std::unique_ptr<Worker>& worker)
            {
                return !worker->is_active();
            });
    }

    std::size_t actual_workers_count = _workers.size() + 1;                                                 // + 1 (the main worker)
    std::size_t required_workers_count = (sessions_count + sessions_per_worker - 1) / sessions_per_worker;  // (a + b - 1) / b (so that the result is rounded up)

    if (actual_workers_count == required_workers_count)
    {
        return;
    }

    if (actual_workers_count > required_workers_count)
    {
        size_t deactivation_index = _workers.size() - 1;
        for (size_t extra_workers = actual_workers_count - required_workers_count; extra_workers > 0; --extra_workers)
        {
            _workers[deactivation_index]->deactivate();
            --deactivation_index;
        }
    }
    else if (actual_workers_count < _max_concurrency && actual_workers_count < required_workers_count)
    {
        _workers.emplace_back(std::make_unique<Worker>());
        _workers.back()->activate(
            [this]
            {
                _context.run_one_for(_timeout);
            });
    }
}
