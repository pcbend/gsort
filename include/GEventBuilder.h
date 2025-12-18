#ifndef __GEVENTBUILDER_H__
#define __GEVENTBUILDER_H__

#include <GThread.h>

#include <globals.h>

#include <vector>


template<class In> //, class Out>
class GEventBuilder : public GThread<std::vector<In> > {
  public:
    using Event = std::vector<In>;
    using Base  = GThread<Event>;

    explicit GEventBuilder(GThread<In>& producer) : fProducer(producer) { 
      //GThread<std::vector<In> >::Register();
      fEvent.reserve(16);
    } 
    virtual ~GEventBuilder() { } 

    void print() override {
      // Determine state string
      const char* state = "STOPPED";
      if (this->IsRunning()) {
        state = this->IsPaused() ? "PAUSED" : "RUNNING";
      }

      // Compute average event size (no division by zero)
      double avg = (fEventCount == 0)
        ? 0.0
        : static_cast<double>(fTotalEventSize) /
        static_cast<double>(fEventCount);

      // Print a clean one-line status
      std::cout 
        << "[GEventBuilder] "
        << "State: " << state
        << " | Events: " << fEventCount
        << " | AvgSize: " << avg
        << " | Partial: " << fEvent.size()
        << " | Queue: " << this->size()
        << "\n";
    }



  private:
    GThread<In>& fProducer;
    Event fEvent;

    uint64_t fTotalEventSize{0};
    uint64_t fEventCount{0};


    bool  Iteration() override {
      In in;
      static uint64_t counter = 0;
      if(!fProducer.pop(in)) {
        if(!fProducer.IsRunning() && fProducer.empty()) {
          if(!fEvent.empty()) { 
            fTotalEventSize += fEvent.size();
            fEventCount++;

            Event out;
            out.swap(fEvent);
            this->push(std::move(out));
            fEvent.clear();
          }
          return false;
        }
        return true;
      }
      //printf("%lu\t ts: %lu\n",counter++,in.timestamp);
      //fflush(stdout);

      //group.
      if(fEvent.empty()) {
        fEvent.push_back(in);
      } else if((in - fEvent.front()) < 1000) {
        fEvent.push_back(in);
      } else {
        //printf(RED " === NEW EVENT === \n" RESET_COLOR); 
        fTotalEventSize += fEvent.size();
        fEventCount++;
        Event out;
        out.swap(fEvent);
        this->push(std::move(out));
        fEvent.clear();
        if(fEvent.capacity()<16) fEvent.reserve(16);
        fEvent.push_back(in);
      }
      return true;
    }
};

#endif
