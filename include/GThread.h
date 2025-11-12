#ifndef __GTHREAD_H__
#define __GTHREAD_H__

#include <TObject.h>

#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>


#include <queue>

#include <Gtypes.h>

template <class T>
class GThread {
  public:
    GThread():fRunning(false),fPaused(false) { } 
    ~GThread() { stop(); } 

    void start() {                        
      if(fRunning.exchange(true)) return;             
        fWorker = std::thread([this]{ this->run(); });
    }                                                 
                                                  
    void pause(bool p=true) {                  
      {std::lock_guard<std::mutex> lk(fMutex); fPaused = p; }
      fCV.notify_all();                               
    }                                                 
                                                  
    void stop()  {                                                           
      bool wasRunning = fRunning.exchange(false);     
      if(wasRunning) {                                
        fCV.notify_all();                             
        if(fWorker.joinable())                        
          fWorker.join();                             
      }                                               
    }            

    //void AttachInput(GThread*);
    //void AttachOutput(GThread*);

    //void SetInputConnectionType(EConnectionType type)  { fInputConnectionType = type;  }
    //void SetOutputConnectionType(EConnectionType type) { fOutputConnectionType = type; }

    template <class U>
    void push(U&& x) {
      {
        std::lock_guard<std::mutex> lk(fMutex);
        fQueue.push(std::forward<U>(x));
      }
      fCV.notify_one();
    }

    template <class... Args>
    void emplace(Args&&... args) {
      {
        std::lock_guard<std::mutex> lk(fMutex);
        fQueue.emplace(std::forward<Args>(args)...);
      }
      fCV.notify_one();
    }

    bool tryPop(T& out) {
      std::lock_guard<std::mutex> lk(fMutex);
      if(fQueue.empty()) return false;
      out = std::move(const_cast<T&>(fQueue.top()));
      fQueue.pop();
      return true;
    }

    bool pop(T& out) {
      std::unique_lock<std::mutex> lk(fMutex);
      fCV.wait(lk, [&]{
        return !fRunning || (!fPaused && !fQueue.empty());
      });
      if (!fRunning && fQueue.empty()) return false;
      if (fPaused || fQueue.empty()) return false;
      out = std::move(const_cast<T&>(fQueue.top()));
      fQueue.pop();
      return true;
    }

    bool peek(T& out) {
      std::lock_guard<std::mutex> lk(fMutex);
      if(fQueue.empty()) return false;
      out = const_cast<T&>(fQueue.top());
      return true;
    }

    size_t size()  const { std::lock_guard<std::mutex> lk(fMutex); return fQueue.size(); }
    bool   empty() const { std::lock_guard<std::mutex> lk(fMutex); return fQueue.empty(); }

  protected: 
    void run() {
      //Initialize();
      while(fRunning) {
        if(!Iteration()) { fRunning = false; break; }
      };
      //Finalize();
      fCV.notify_all();
    }

  private:
    //virtual void Initialize() = 0;
   virtual bool Iteration() = 0;
    //virtual void Finalize()   = 0;
    
    mutable std::mutex fMutex;
    std::condition_variable fCV;

    std::thread fWorker;
    std::atomic_bool fRunning;
    std::atomic_bool fPaused;

    std::priority_queue<T,std::vector<T> > fQueue;
};

#endif
