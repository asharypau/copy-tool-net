#ifndef SERVER_WORKERS_POOL
#define SERVER_WORKERS_POOL

#include <boost/asio.hpp>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

namespace Server
{
    class WorkersPool
    {
    private:
        class Worker
        {
        public:
            Worker();
            ~Worker();

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
            }

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
            }

        private:
            bool _is_active;
            std::optional<std::thread> _worker;
        };

    public:
        explicit WorkersPool(boost::asio::io_context& context);

        void resize(std::size_t sessions_count);

    private:
        static const unsigned int sessions_per_worker = 2;

        unsigned int _max_concurrency;
        std::chrono::milliseconds _timeout;
        std::vector<std::unique_ptr<Worker>> _workers;
        boost::asio::io_context& _context;
    };
}

#endif  // SERVER_WORKERS_POOL
