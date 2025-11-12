#ifndef __GTHREAD_H__
#define __GTHREAD_H__

#include <TObject.h>

#include <atomic>
#include <condition_variable>
#include <thread>

#include <queue>

#include <Gtypes.h>

class GThread {
  public:
    GThread();
    virtual ~GThread();

    virtual bool Iteration() = 0;

    void doStart();
    void doPause() { fPaused = !fPaused; }
    void doStop()  { fRunning = false;   }

    void AttachInput(GThread*);
    void AttachOutput(GThread*);

    void SetInputConnectionType(EConnectionType type)  { fInputConnectionType = type;  }
    void SetOutputConnectionType(EConnectionType type) { fOutputConnectionType = type; }
    EConnectionType GetInputConnectionType()    const  { return fInputConnectionType;  }
    EConnectionType GetOutputConnectionType()   const  { return fOutputConnectionType; }
  private:

    virtual void Initialize() = 0;
    void Loop();
    virtual void Finalize()   = 0;

    EConnectionType fInputConnectionType;
    EConnectionType fOutputConnectionType;

    std::thread fThread;
    std::atomic_bool fRunning;
    std::atomic_bool fPaused;
    std::condition_variable fPausedWait;
    std::mutex fPauseMutex;

    std::queue<std::unique_ptr<TObject> > fQueue;

  ClassDef(GThread,0)
};

#endif
