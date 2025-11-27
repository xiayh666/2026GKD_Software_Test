#pragma once
#include "Task.hpp"
#include <atomic>
#include <memory>

class GainTask : public Task<GainTask> {
private:
  std::shared_ptr<std::atomic<int>> k = std::make_shared<std::atomic<int>>(1);

public:
  GainTask(std::string key, Data::Ptr in, Data::Ptr out);
  ~GainTask();
  void run();
  void callback(int msg);
};