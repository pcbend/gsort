
#include <GThread.h>
#include <chrono>
#include <iostream>
#include <thread>
 


GThread::GThread():fRunning(false),fPaused(false) { 
  fInputConnectionType  = EConnectionType::kUNKNOWN; 
  fOutputConnectionType = EConnectionType::kUNKNOWN;
} 

GThread::~GThread() { } 

void GThread::doStart() {
  fRunning = true;
  fThread = std::thread(&GThread::Loop,this);
  fThread.detach();
}

/*
bool GThread::Iteration() { 
  static int count=0;
  printf(" counting %i...\n",count++);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return true;
}
*/


//void GThread::Initialize() {   }

void GThread::Loop()      {   
  Initialize();
  while(fRunning) {
    std::unique_lock<std::mutex> lock(fPauseMutex);
    while(fPaused && fRunning) {
      fPausedWait.wait_for(lock, std::chrono::milliseconds(100));
    }
    bool success = Iteration();
    if(!success) {
      fRunning=false;
      break;
    }
  }
  Finalize();
}

//void GThread::Finalize()  {   }


void GThread::AttachInput(GThread *other)  { 

}

void GThread::AttachOutput(GThread *other) { 

}
