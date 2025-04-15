#ifndef SERVER_WORKERS_POOL
#define SERVER_WORKERS_POOL

#include <boost/asio.hpp>
#include <memory>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

#include "../utils/Logger.h"

namespace Server
{
    class WorkersPool
    {
    private:
        class ManagerTask
        {
        public:
            bool stop_signal = false;
            bool clear_signal = false;
            bool extend_signal = false;
            unsigned int required_pool_size = 0;
        };

        class Worker
        {
        public:
            template <class Func>
            explicit Worker(Func&& func)
                : _is_active(false),
                  _worker(),
                  _hasher()
            {
                activate(std::forward<Func>(func));
            }

            ~Worker()
            {
                if (_worker && _worker->joinable())
                {
                    _worker->join();
                }
            }

            // Disallow copying and assignment.
            Worker(const Worker&) = delete;
            Worker& operator=(const Worker&) = delete;

            // Disallow movement and assignment.
            Worker(Worker&& other) = delete;
            Worker& operator=(Worker&& other) = delete;

            bool is_active() const noexcept
            {
                return _is_active;
            }

            void deactivate() noexcept
            {
                _is_active = false;

                Logger::debug(std::format("Worker {} deactivated.", get_id()));
            }

        private:
            template <class Func>
            void activate(Func&& func)
            {
                _is_active = true;
                _worker.emplace(
                    [this, func = std::forward<Func>(func)]
                    {
                        while (_is_active)
                        {
                            func();
                        }
                    });

                Logger::debug(std::format("Worker {} activated.", get_id()));
            }

            std::size_t get_id() const noexcept
            {
                return _worker ? _hasher(_worker->get_id()) : 0;
            }

            bool _is_active;
            std::optional<std::thread> _worker;
            std::hash<std::thread::id> _hasher;
        };

    public:
        explicit WorkersPool(boost::asio::io_context& context);
        ~WorkersPool();

        void resize(const std::size_t sessions_count);

    private:
        std::size_t get_required_pool_size(const std::size_t sessions_count);
        void manage();

        static const unsigned int sessions_per_worker = 2;

        std::mutex _mtx;
        std::condition_variable _cv;
        std::chrono::milliseconds _timeout;
        const unsigned int _max_pool_size;
        unsigned int _curr_pool_size;
        std::vector<std::unique_ptr<Worker>> _pool;
        std::thread _manager;
        std::queue<ManagerTask> _manager_tasks;
        boost::asio::io_context& _context;
    };

}

#endif  // SERVER_WORKERS_POOL
