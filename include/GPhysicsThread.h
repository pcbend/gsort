#ifndef __GPHYSICSTHREAD_H__
#define __GPHYSICSTHREAD_H__

#include <GThread.h>
#include <GDetector.h>
#include <Gtypes.h>

#include <Unpacker.h>

#include <vector>
#include <cstdint>
#include <thread>
#include <chrono>

template<class In>
class GPhysicsThread : public GThread<std::vector<std::unique_ptr<GDetector> > > {
  public:
    using Event = std::vector<In>;
    using Base  = GThread<std::vector<std::unique_ptr<GDetector> > >;

    explicit GPhysicsThread(GThread<Event>& producer, const FileInfo &info) 
      : fProducer(producer), fInfo(info) { }
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
    const FileInfo &fInfo;
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

      //Unpack(ev,fInfo);

      //thread_local std::vector<GDetector> dets;
      //dets.clear();
      //dets.reserve(4);
      std::vector<std::unique_ptr<GDetector> > dets = Unpack(ev,fInfo);
      this->emplace(std::move(dets));
      dets.reserve(4);
      return true;
    }

};


#endif
