#pragma once
#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

struct Data {
  using Val = int;
  using Ptr = std::shared_ptr<std::atomic<Val>>;
  static inline Ptr create_ptr(Val v) {
    return std::make_shared<std::atomic<Val>>(v);
  }
};


class Logger {
private:
  std::mutex mtx_;
  std::deque<std::string> log_deque_;
  std::condition_variable should_log_;

public:
  void log_task() {
    std::thread([this]() {
      while (true) {
        std::unique_lock<std::mutex> lock(mtx_);
        should_log_.wait(lock, [this](){return !log_deque_.empty();});
        std::cout << log_deque_.front() << std::endl;
        log_deque_.pop_front();
      }
    }).detach();
  }

  void log(std::string msg) {
    std::lock_guard<std::mutex> lock(mtx_);
    log_deque_.push_back(std::move(msg));
    should_log_.notify_one();
  }
};