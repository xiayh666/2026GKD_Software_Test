#include "tasks/TaskManager.hpp"
#include "common.hpp"
#include "tasks/DelayBufferTask.hpp"
#include <chrono>
#include <format>
#include <memory>
#include <string>
#include <thread>
#include <type_traits>

using namespace std::chrono_literals;

extern std::unique_ptr<Logger> logger;

void TaskManager::set_input_data(Data::Val data) {
  *buffers[buffers.size() - 1].p_data = data;
}

Data::Val TaskManager::get_output_data() { return *buffers[0].p_data; }

void TaskManager::output_task() {
  std::thread([this]() {
    while (true) {
      Data::Val v;
      if ((v = *buffers[0].p_data) != 0) {
        *buffers[0].p_data = 0;
        logger->log(std::format("output: {}", std::to_string(v)));
      }
      std::this_thread::sleep_for(50us);
    }
  }).detach();
}

void TaskManager::pop() {
  if (buffers.size() <= 1)
    return;
  std::string key = buffers.back().key;
  logger->log(key);
  std::visit([](auto &task) { task->stop(); }, tasks.at(key));
  tasks.erase(key);
  buffers.pop_back();
}
