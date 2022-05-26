#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads_count)
{
    m_threads_count_ = threads_count;
    if (threads_count > std::thread::hardware_concurrency())
    {
        m_threads_count_ = std::thread::hardware_concurrency();
    }
    if (threads_count < 1)
    {
        m_threads_count_ = 1;
    }

    m_threads_.resize(m_threads_count_);

    Init_();
}

ThreadPool::~ThreadPool() {
    Join();
}

void ThreadPool::Submit(std::function<void()> fn) {
    Submit(
        [fn = std::move(fn)](std::stop_token) {
        fn();
    });
}

void ThreadPool::Submit(std::function<void(std::stop_token)> fn) {
    if (m_no_more_tasks_ or m_stop_requested_) {
        throw std::logic_error{ "can't submit task, thread pool is closed" };
    }
    auto lg = std::lock_guard{ m_mtx_ };
    m_queue_.emplace_back(std::move(fn));
    m_cond_var_.notify_one();
}

void ThreadPool::Join() {
    m_no_more_tasks_ = true;
    m_cond_var_.notify_all();
    for (auto& thr : m_threads_) {
        if (thr.joinable()) {
            thr.join();
        }
    }
}

void ThreadPool::Stop() {
    m_no_more_tasks_ = true;
    m_stop_requested_ = true;
    for (auto& thr : m_threads_) {
        thr.request_stop();
    }
    m_cond_var_.notify_all();
}

void ThreadPool::Init_() {
    for (auto i = 0; i < m_threads_count_; ++i) {
        m_threads_[i] = std::jthread{
            [this, i](std::stop_token stop_token) {
                ThreadLoop_(stop_token, i);
            }
        };
    }
}

void ThreadPool::ThreadLoop_(std::stop_token stop_token, int32_t thread_id) {
    auto&& queue = m_queue_;
    auto lock = std::unique_lock{ m_mtx_, std::defer_lock };
    while (not stop_token.stop_requested()) {

        lock.lock();
        m_cond_var_.wait(lock, [this, &queue, &stop_token] {
            return not queue.empty() or stop_token.stop_requested() or m_no_more_tasks_;
            });
        if (stop_token.stop_requested() or m_no_more_tasks_ and queue.empty()) {
            break;
        }
        auto fn = std::move(queue.front());
        queue.pop_front();
        lock.unlock();

        fn(stop_token);
        std::this_thread::yield();
    }
}