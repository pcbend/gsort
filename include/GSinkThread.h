#ifndef __GSINKTHREAD_H__
#define __GSINKTHREAD_H__

#include "GThread.h"
#include <cstdint>
#include <thread>
#include <chrono>

template<class In>
class GSinkThread : public GThread<int> {
  public:
    explicit GSinkThread(GThread<In>& producer) : fProducer(producer) { }

    ~GSinkThread() override = default;

    uint64_t count() const { return fCount; }

    void print() override {
        const char* state = "STOPPED";
        if (this->IsRunning()) state = this->IsPaused() ? "PAUSED" : "RUNNING";

        std::cout << "[GSink] State: " << state
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
