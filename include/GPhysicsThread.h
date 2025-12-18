#ifndef __GPHYSICSTHREAD_H__
#define __GPHYSICSTHREAD_H__

#include "GThread.h"
#include <cstdint>
#include <thread>
#include <chrono>

template<class In>
class GPhysicsThread : public GThread<GDetector> {
  public:
    explicit GPhysicsThread(GThread<In>& producer) : fProducer(producer) { }

    ~GPhysicsThread() override = default;

    uint64_t count() const { return fCount; }

    void print() override {
        const char* state = "STOPPED";
        if (this->IsRunning()) state = this->IsPaused() ? "PAUSED" : "RUNNING";

        std::cout << "[GPhysics] State: " << state
                  << " | Drained: " << fCount
                  << "\n";
    }

  private:
    GThread<In>& fProducer;
    uint64_t fCount{0};


    bool Iteration() override {
      In tmp;
      if (!fProducer.pop(tmp)) {
        if (!fProducer.IsRunning() && fProducer.empty()) {
            return false; // stop this sink thread
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
      }
      fCount++;
      return true;
    }

};


#endif
