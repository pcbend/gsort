#include <GPhysicsThread.h>
#include <Unpacker.h>

#include <chrono>
#include <iostream>
#include <thread>

GPhysicsThread::GPhysicsThread(GThread<Event>& producer, const FileInfo& info)
  : fProducer(producer), fInfo(info) {}

void GPhysicsThread::print() {
  const char* state = "STOPPED";
  if (this->IsRunning())
    state = this->IsPaused() ? "PAUSED" : "RUNNING";

  std::cout << "[GPhysics] State: " << state
            << " | Events: " << fEvents
            << " | OutQ: " << this->size()
            << "\n";
}

bool GPhysicsThread::Iteration() {
  Event ev;

  if (!fProducer.pop(ev)) {
    if (!fProducer.IsRunning() && fProducer.empty())
      return false;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return true;
  }

  fEvents++;

  auto dets = Unpack(ev, fInfo);
  this->emplace(std::move(dets));

  return true;
}
