#include "ThreadPool.hpp"

namespace SimpleNet
{

ThreadPool::ThreadPool(size_t threads) : stop_requested_(false)
{
    for (size_t i = 0; i < threads; ++i)
    {
        workers_.emplace_back([this]
        {
            while (true)
            {
                std::unique_ptr<std::function<void()>> current_task;

                {
                    std::unique_lock<std::mutex> lock(tasks_mutex_);
                    has_tasks_.wait(lock, [this]
                    {
                        return stop_requested_ || !task_queue_.empty();
                    });

                    if (stop_requested_ && task_queue_.empty()) return;

                    current_task = std::move(task_queue_.front());
                    task_queue_.pop();
                }

                (*current_task)();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(tasks_mutex_);
        stop_requested_ = true;
    }

    has_tasks_.notify_all();

    for (auto& worker : workers_)
    {
        if (worker.joinable()) worker.join();
    }
}

} // namespace SimpleNet
