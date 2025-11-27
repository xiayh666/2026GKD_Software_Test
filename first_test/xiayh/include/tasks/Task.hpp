#pragma once
#include "common.hpp"
#include <atomic>
#include <iostream>
#include <memory>

template <typename Derived> class Task {
private:
  Data::Ptr p_in_;
  Data::Ptr p_out_;

protected:
  constexpr static Data::Val None = 0; // 无数据
  std::shared_ptr<std::atomic<bool>> stop_ =
      std::make_shared<std::atomic<bool>>(false);
  std::string key_;

  inline Data::Val get_in() { return *p_in_; }
  inline Data::Val get_out() { return *p_out_; }
  inline void set_in(Data::Val val) { *p_in_ = val; }
  inline void set_out(Data::Val val) { *p_out_ = val; }

  inline Data::Val take_in() { return p_in_->exchange(0); }

public:
  Task(std::string key, Data::Ptr p_in, Data::Ptr p_out) {
    *stop_ = false;
    key_ = key;
    p_in_ = p_in;
    p_out_ = p_out;
  }
  ~Task() {}
  void callback(int msg) { static_cast<Derived *>(this)->callback(msg); }
  void run() { static_cast<Derived *>(this)->run(); }
  void stop() { *stop_ = true; }
  std::shared_ptr<Derived> get_shared_ptr() {
    return std::make_shared<Derived>(static_cast<Derived const &>(*this));
  }
};