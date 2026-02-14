#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace SimpleNet
{

class ThreadPool
{
public:
    ThreadPool(size_t threads);

    template<class F>
    void enqueue(F&& task)
    {
        auto task_wrapper = std::make_unique<std::function<void()>>(std::forward<F>(task));
        {
            std::lock_guard<std::mutex> lock(tasks_mutex_);
            task_queue_.push(std::move(task_wrapper));
        }
        has_tasks_.notify_one();
    }

    ~ThreadPool();

private:
    std::vector<std::thread> workers_;
    std::queue<std::unique_ptr<std::function<void()>>> task_queue_;

    std::mutex tasks_mutex_;
    std::condition_variable has_tasks_;
    std::atomic<bool> stop_requested_;
};

} // namespace SimpleNet
