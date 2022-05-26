#pragma once
#include <atomic>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>


class ThreadPool {
public:
    ThreadPool(size_t threads_count);

    ThreadPool(ThreadPool const&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool const&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    ~ThreadPool();

public:
    void Submit(std::function<void()> fn);
    void Submit(std::function<void(std::stop_token)>);
    void Join();
    void Stop();

private:
    void Init_();
    void ThreadLoop_(std::stop_token stop_token, int32_t thread_id);

private:
    std::vector<std::jthread> m_threads_;
    size_t m_threads_count_{ 1 };
    std::deque<std::function<void(std::stop_token)>> m_queue_;
    std::mutex m_mtx_;
    std::condition_variable m_cond_var_;
    std::atomic_bool m_no_more_tasks_{ false };
    std::atomic_bool m_stop_requested_{ false };
};