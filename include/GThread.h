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

struct GThreadInfo {
    std::string className;
    std::thread::id threadId; // Track the worker thread's ID
};

class GThreadManager {
 public:
    typedef void (*StatusPrinter)();
    typedef void (*CommandDispatcher)();

    static std::map<std::string, StatusPrinter> fAllThreadPrinters;
    static std::map<std::string, GThreadManager::CommandDispatcher> fAllThreadCommanders;
    static std::mutex fManagerMutex;

    static void RegisterPrinter(const std::string& typeName, StatusPrinter printer) {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        fAllThreadPrinters[typeName] = printer;
    }
    
    static void RegisterCommander(const std::string& typeName, CommandDispatcher commander) {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        fAllThreadCommanders[typeName] = commander;
    }

    static void StopAll() {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        std::cout << "\n=== Dispatching StopAll command to all active thread types... ===" << std::endl;
        for (const auto& pair : fAllThreadCommanders) {
            std::cout << "Stopping threads of type: " << pair.first << std::endl;
            // The commander function will iterate over all instances of type T and call stop()
            pair.second(); 
        }
        std::cout << "Stop dispatch complete." << std::endl;
    }

    static void JoinAll() {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        std::cout << "\n=== Dispatching JoinAll command to all active thread types... ===" << std::endl;
        for (const auto& pair : fAllThreadCommanders) {
            // The commander function will iterate over all instances of type T and call join()
            pair.second(); 
        }
        std::cout << "Join dispatch complete." << std::endl;
    }

    static void PrintAllStatuses() {
        std::lock_guard<std::mutex> lock(fManagerMutex);
        std::cout << "=== Global Thread Status Report ===" << std::endl;
        if (fAllThreadPrinters.empty()) {
             std::cout << "No thread types registered." << std::endl;
        }
        for (const auto& pair : fAllThreadPrinters) {
            pair.second();
        }
        std::cout << "===================================" << std::endl;
    }
};

// --- GThreadManager Static Member Initialization ---
inline std::map<std::string, GThreadManager::StatusPrinter> GThreadManager::fAllThreadPrinters;
inline std::map<std::string, GThreadManager::CommandDispatcher> GThreadManager::fAllThreadCommanders;
inline std::mutex GThreadManager::fManagerMutex;


template <class T>
class GThread {
 public:
    static std::map<GThread<T>*, GThreadInfo> fThreadMap;
    static std::mutex fMapMutex;
    
    GThread():fRunning(false),fPaused(false) { AddThread(this);} 
    ~GThread() { stop(); RemoveThread(this);} 

    static bool Register() {
        GThreadManager::RegisterPrinter(
            typeid(T).name(), 
            &GThread<T>::PrintStatus
        );
        GThreadManager::RegisterCommander(
            typeid(T).name(),
            &GThread<T>::StopAll
        );
        return true;
    }

    static void PrintStatus() {
        std::lock_guard<std::mutex> lock(fMapMutex);
        
        std::cout << "\n--- GThread Status for <" << typeid(T).name() << "> ---" << std::endl;
        std::cout << "Total active threads of this type: " << fThreadMap.size() << std::endl;
        
        if (fThreadMap.empty()) {
            std::cout << "No active instances found." << std::endl;
            return;
        }

        for (const auto& pair : fThreadMap) {
            GThread<T>* threadPtr = pair.first;
            const GThreadInfo& info = pair.second;

            // Determine the current state
            std::string state = "STOPPED";
            // Check threadPtr validity (just in case) and running flag
            if (threadPtr && threadPtr->IsRunning()) {
                state = threadPtr->IsPaused() ? "PAUSED" : "RUNNING";
            }

            // Print the status of an individual thread instance
            std::cout << "[Instance] | Class: " << info.className 
                      << " | ID: " << info.threadId
                      << " | State: " << state
                      << " | Queue Size: " << threadPtr->size() << std::endl;
        }
    }

    static void StopAll() {
        std::lock_guard<std::mutex> lock(fMapMutex);
        for (const auto& pair : fThreadMap) {
            GThread<T>* threadPtr = pair.first;
            // Signal the thread to stop gracefully
            threadPtr->stop(); 
        }
    }

    static void JoinAll() {
        std::lock_guard<std::mutex> lock(fMapMutex);
        for (const auto& pair : fThreadMap) {
            GThread<T>* threadPtr = pair.first;
            // We can't call fWorker.join() directly, but we can call a join wrapper
            if (threadPtr->fWorker.joinable()) {
                threadPtr->fWorker.join();
            }
        }
    }


    bool IsRunning() const { return fRunning.load(); }
    bool IsPaused() const { return fPaused.load(); }

    void start() {                          
      if(fRunning.exchange(true)) return;              
        fWorker = std::thread([this]{ this->run(); });
        // 3. Update thread ID after the worker thread is launched
        UpdateThreadID(this, fWorker.get_id());
    }           
    // ... (rest of the public methods remain the same) ...

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
    bool  empty() const { std::lock_guard<std::mutex> lk(fMutex); return fQueue.empty(); }

 protected: 
    void run() {
      //Initialize();
      while(fRunning) {
        if(!Iteration()) { fRunning = false; break; }
      };
      //Finalize();
      fCV.notify_all();
      UpdateThreadID(this, std::thread::id());
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

 private:
    static void AddThread(GThread<T>* threadPtr) {
        std::lock_guard<std::mutex> lock(fMapMutex);
        GThreadInfo info;
        info.className = typeid(*threadPtr).name();
        fThreadMap[threadPtr] = info;
    }

    static void RemoveThread(GThread<T>* threadPtr) {
        std::lock_guard<std::mutex> lock(fMapMutex);
        fThreadMap.erase(threadPtr);
    }
    
    static void UpdateThreadID(GThread<T>* threadPtr, std::thread::id id) {
        std::lock_guard<std::mutex> lock(fMapMutex);
        auto it = fThreadMap.find(threadPtr);
        if (it != fThreadMap.end()) {
            it->second.threadId = id;
        }
    }

};

template <class T>
std::map<GThread<T>*, GThreadInfo> GThread<T>::fThreadMap;

template <class T>
std::mutex GThread<T>::fMapMutex;


#endif
