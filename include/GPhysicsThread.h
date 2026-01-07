#ifndef __GPHYSICSTHREAD_H__
#define __GPHYSICSTHREAD_H__

#include <GThread.h>
#include <GDetector.h>
#include <Gtypes.h>

#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>

template<class In>
class GPhysicsThread : public GThread<GDetector> {
  public:
    using Event = std::vector<In>;
    using Base  = GThread<GDetector>;

    explicit GPhysicsThread(GThread<Event>& producer, const FileContext &ctx) 
      : fProducer(producer), fCtx(ctx) { }
    //explicit GPhysicsThread(GThread<In>& producer) : fProducer(producer) { }

    ~GPhysicsThread() override = default;

    uint64_t count() const { return fEvents; }

    void print() override {
        const char* state = "STOPPED";
        if (this->IsRunning()) state = this->IsPaused() ? "PAUSED" : "RUNNING";

        std::cout << "[GPhysics] State: " << state
                  << " | Events: " << fEvents
        //          << " | AvgHits: " << avg
                  << " | OutQ: " << this->size()
                  << "\n";
    }



  private:
    GThread<Event>& fProducer;
    const FileContext &fCtx;
    uint64_t fEvents{0};

    bool Iteration() override {
      Event ev;
      if (!fProducer.pop(ev)) {
        if (!fProducer.IsRunning() && fProducer.empty()) {
            return false; // stop this sink thread
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
      }
      fEvents++;

      GDetector det;
      det.Clear();
      //for (const Rec &r : ev) {
      //  det.SetTimestamp(r.timestamp);
        det.Unpack(ev,fCtx);
      //}
      this->emplace(std::move(det));
      return true;
    }

};


#endif
