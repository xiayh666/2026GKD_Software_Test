#include "Tasks/GainTask.hpp"
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;
extern std::unique_ptr<Logger> logger;

GainTask::GainTask(std::string key, Data::Ptr in, Data::Ptr out)
    : Task<GainTask>(key, in, out) {}
GainTask::~GainTask() {};

void GainTask::run() {
  std::cout << "Gt run" << std::endl;
  std::thread([this]() {
    while (true) {
      if (*stop_) {
        std::cout << "Gt stop" << std::endl;
        return;
      }
      Data::Val v;
      if ((v = take_in()) != None) {
        Data::Val res = v * (*k);
        set_out(res);
        logger->log(std::format("write Gain-{}: {}", key_, res));
      }
      std::this_thread::sleep_for(1ms);
    }
  }).detach();
}

void GainTask::callback(int msg) { *k = msg; }
