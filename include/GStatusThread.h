#ifndef __GSTATUSTHREAD_H__
#define __GSTATUSTHREAD_H__

#include "GThread.h"

#include <chrono>
#include <thread>

#include <globals.h>

class GStatusThread : public GThread<int> {
  public:
    GStatusThread() : fStartTime(std::chrono::steady_clock::now()) { } 
    virtual ~GStatusThread() = default;

  private: 
    
    int  fLastLines{0};
    bool fLiveMode{true};

    std::chrono::steady_clock::time_point fStartTime;

    void print() override { 
      printf("\n");
      return;
    }

   
    void printElapsed() {
      auto now  = std::chrono::steady_clock::now();
      auto secs = std::chrono::duration_cast<std::chrono::seconds>(now - fStartTime);
      auto h    = std::chrono::duration_cast<std::chrono::hours>(secs);
      secs -= h;
      auto m = std::chrono::duration_cast<std::chrono::minutes>(secs);
      secs -= m;
      printf("Run time: %02lld:%02lld:%02lld\n",
               static_cast<long long>(h.count()),
               static_cast<long long>(m.count()),
               static_cast<long long>(secs.count()));
    }




    bool Iteration() override {
      if(!IsRunning()) {
        printf("Status Thread stopping.\n");
        return false;
      }
      if(IsPaused()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return true;
      }
      if(fLiveMode)
        for(int i=0;i<fLastLines;i++)
          printf(CURSOR_UP CLEAR_LINE);

      printElapsed();
      printf("total threads[%zu]\n",GThreadManager::RunningThreadCount()); // the last one standing.
      //printf("threads[%zu]\n",GThreadManager::RunningThreadCount());
      fLastLines = GThreadManager::PrintAll() + 2; 
      for(int i=0;i<10;i++) {
        if(!IsRunning() || IsPaused())
          break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
 
      if(GThreadManager::RunningThreadCount()==1) // the last one standing.
        return false;
        //stop();

      return true;
    }
};
#endif
