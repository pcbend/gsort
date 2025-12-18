#ifndef __GTHREAD_H__
#define __GTHREAD_H__

#include <TObject.h>

#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>
#include <map>
#include <mutex>
#include <iostream>
#include <typeinfo> 
#include <string>   

#include <queue>

#include <Gtypes.h>

class GThreadBase {
  public:
    virtual ~GThreadBase()    = default;
    virtual  void start()     = 0;
    virtual  void stop()      = 0;
    virtual  void pause(bool) = 0;
    virtual  void print()     = 0;
    virtual  void join()      = 0;
    virtual  void detatch()   = 0;

    virtual bool IsRunning() const =0;
    virtual bool IsPaused() const =0;
    virtual bool IsEmpty()  const =0;
    virtual bool HasData() const =0;
    virtual bool IsIdle() const =0;
};


class GThreadManager {
 public:
    static std::mutex fManagerMutex;
    static std::vector<GThreadBase*> fThreads;

    static void RegisterThread(GThreadBase *t) {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        fThreads.push_back(t);
    }
    
    static void UnregisterThread(GThreadBase *t) {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        auto it = std::remove(fThreads.begin(),fThreads.end(),t);
        fThreads.erase(it,fThreads.end());
    }

    static void StopAll() {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        std::cout << "\n=== Dispatching StopAll command to all active thread types... ===" << std::endl;
        for(auto* t : fThreads) t->stop(); 
        std::cout << "Stop dispatch complete." << std::endl;
    }

    static void JoinAll() {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        std::cout << "\n=== Dispatching JoinAll command to all active thread types... ===" << std::endl;
        for(auto* t : fThreads) t->join();
        std::cout << "Join dispatch complete." << std::endl;
    }

    static int PrintAll() {
      std::lock_guard<std::mutex> lock(fManagerMutex);
      int lines = 2;
      std::cout << "=== Global Thread Status Report ===" << std::endl;
      if (fThreads.empty()) {
          std::cout << "No active threads.\n";
          lines++;
      }
      for (auto* t : fThreads) {
          t->print();   // virtual call
          lines++;
      }
      std::cout << "===================================" << std::endl;
      return lines;
    }

    static size_t RunningThreadCount() {
      std::lock_guard<std::mutex> lock(fManagerMutex);
      size_t count = 0;
      for (auto* t : fThreads) {
        if (t && !t->IsIdle() ) 
            count++;
      }
      return count;
    }


};

inline std::mutex GThreadManager::fManagerMutex;
inline std::vector<GThreadBase*> GThreadManager::fThreads;


template <class T>
class GThread : GThreadBase  {
 public:
    
    GThread():fRunning(false),fPaused(false) { GThreadManager::RegisterThread(this);} 
    ~GThread() override { 
      stop();
      GThreadManager::UnregisterThread(this);
    } 

    void print() override {
      std::string state = "STOPPED";
      if (IsRunning()) state = IsPaused() ? "PAUSED" : "RUNNING";
      std::cout << "[GThread<" << typeid(T).name() << ">] "
                << "| State: " << state
                << " | Queue Size: " << size()
                << "\n";
    }

    void join() override { 
      if(fWorker.joinable())
        fWorker.join();
      return;
    }
    
    void detatch() override {
      if(fWorker.joinable())
        fWorker.detach();
      return;
    }



    bool IsRunning() const override { return fRunning.load(); }
    bool IsPaused() const override { return fPaused.load(); }
    bool IsEmpty()  const override { 
      std::lock_guard<std::mutex> lk(fMutex);
      return fQueue.empty();
    }
    bool HasData() const override { return !IsEmpty(); }
    bool IsIdle()  const override { return !IsRunning() && IsEmpty(); }


    void start() override {                          
      if(fRunning.exchange(true)) return;              
        fWorker = std::thread([this]{ this->run(); });
        // 3. Update thread ID after the worker thread is launched
        //updateThreadID(this, fWorker.get_id());
    }           
    // ... (rest of the public methods remain the same) ...

    void pause(bool p=true) override {           
      {std::lock_guard<std::mutex> lk(fMutex); fPaused = p; }
      fCV.notify_all();                           
    }           
                
    void stop()  override {                                       
      bool wasRunning = fRunning.exchange(false);      
      //if(wasRunning) {                          
      fCV.notify_all();                           
      join();
      //}                          
    }           

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

    virtual bool pop(T& out) {
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

    bool popReady() {
      std::lock_guard<std::mutex> lk(fMutex);
      if (!fRunning) return false;
      if (fPaused)   return false;
      if (fQueue.empty()) return false;

      return true;
    }

    bool peek(T& out) {
      std::lock_guard<std::mutex> lk(fMutex);
      if(fQueue.empty()) return false;
      out = const_cast<T&>(fQueue.top());
      return true;
    }

    size_t size()  const { std::lock_guard<std::mutex> lk(fMutex); return fQueue.size(); }
    bool  empty() const { std::lock_guard<std::mutex> lk(fMutex); return fQueue.empty(); }

 protected: 
    void run() {
      //Initialize();
      while(fRunning) {
        if(!Iteration()) { fRunning = false; break; }
      };
      //Finalize();
      fCV.notify_all();
      //UpdateThreadID(this, std::thread::id());
    }

 private:
    //virtual void Initialize() = 0;
    virtual bool Iteration() = 0;
    //virtual void Finalize()  = 0;
    
    mutable std::mutex fMutex;
    std::condition_variable fCV;

    std::thread fWorker;
    std::atomic_bool fRunning;
    std::atomic_bool fPaused;

    std::priority_queue<T,std::vector<T> > fQueue;
};



#endif
