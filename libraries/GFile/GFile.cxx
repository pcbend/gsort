
#include <GFile.h>

#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <TTimer.h>

#include <GBanks.h> 

#include <globals.h>


GFile::GFile(std::string inFile) : fOffset(0),fMapFile(0),fMapOpened(false),fData(0) {
  fFd = open(inFile.c_str(), O_RDONLY);
  if(fFd==-1) return;
  struct stat sb;
  if(fstat(fFd,&sb)==-1) return;

  fSize = sb.st_size;
                        //%lld
  printf("Opened file %s [%.02f GB]\n",inFile.c_str(),fSize/1024./1024./1024.);  
  
  //GThread<Rec>::Register();  
  
  fMapFile = mmap(NULL,fSize,PROT_READ,MAP_PRIVATE,fFd,0);
  if(!(fMapFile == MAP_FAILED)) {
    fMapOpened = true;
    fData = static_cast<char*>(fMapFile);//
    fCtx.base = fData;
    fCtx.size = fSize;
  }

  SetFileType(inFile);

}

GFile::~GFile() {
  if(fMapOpened) 
    munmap(fMapFile,fSize);
  if(fFd>0) close(fFd); 
}

void GFile::SetFileType(std::string& fname) {
  //do something more useful.
  fFileType = kGEB;

}

void GFile::ReadSome() { 
  //check file type

  GEBHeader *header;
  header = (GEBHeader*)fData;
  
  std::cout << *header << std::endl;
  fData = fData + sizeof(GEBHeader) + header->size;
  header = (GEBHeader*)fData;
  std::cout << *header << std::endl;

}

void GFile::Read() {
  auto start     = std::chrono::steady_clock::now();
  auto lastPrint = std::chrono::steady_clock::now();
  auto timeEllapsed = std::chrono::duration_cast<std::chrono::seconds>(lastPrint-start);
  const std::chrono::seconds interval(1); // 1 second interval

  GEBHeader *header;
  while(fOffset < fSize) {
    Iteration();

    if(std::chrono::steady_clock::now()-lastPrint>interval) {
      lastPrint = std::chrono::steady_clock::now();
      timeEllapsed = std::chrono::duration_cast<std::chrono::seconds>(lastPrint-start);
      printf(" %lld \t read %.02f / %.02f  GB               \r",timeEllapsed.count(),fOffset/1024./1024./1024.,fSize/1024./1024./1024.);
      fflush(stdout);
    }
  }
  timeEllapsed = std::chrono::duration_cast<std::chrono::seconds>(lastPrint-start);
  printf(CLEAR_LINE " read %.02f / %.02f  GB  -  %.02f mins\n",fOffset/1024./1024./1024.,fSize/1024./1024./1024.,timeEllapsed.count()/60.);
  printf("done!\n");
}

void GFile::Sort() { 
  start();
}

bool GFile::Iteration() { 
  if(gShutdown) { 
    fDoneReading=true;
    return false;
  }

  constexpr size_t kHighWater = 2'000'000; // tune
  while (this->size() > kHighWater && this->IsRunning() && !this->IsPaused()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  if(fOffset>=fSize) {
    fDoneReading = true;
    return false;
  }
  GEBHeader *header = (GEBHeader*)(fData+fOffset);  //most recent...
  
  static uint64_t seq=0;
  this->emplace(Rec{static_cast<uint64_t>(header->timestamp),
                    static_cast<uint32_t>(header->type),
                    static_cast<uint32_t>(header->size),
                    static_cast<uint64_t>(fOffset),
                    seq++});
 if(header->timestamp>fLastTimestamp)
   fLastTimestamp = header->timestamp;

 fOffset += sizeof(GEBHeader) + header->size;
  
  return true; 
} 

bool GFile::pop(Rec &out) {
  constexpr uint64_t kWindowTicks = 5ULL * 100'000'000ULL; // 5 seconds @ 10ns
  Rec top;
  while(1) { 
    if(!IsRunning() && empty()) return false;
    if(!peek(top)) return GThread<Rec>::pop(out);
    uint64_t oldest = top.timestamp;
    if (fDoneReading || (fLastTimestamp - oldest) >= kWindowTicks) break;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));    
  }
  return GThread<Rec>::pop(out);
}

void GFile::print() { 
  std::string state = "STOPPED";
  if (IsRunning()) state = IsPaused() ? "PAUSED" : "RUNNING";
  std::cout << "[GFile] "
            << "| State: " << state
            << " | Offset: " << fOffset/1024./1024. << "/" << fSize/1024./1024. << " MB"
            << " | Queue Size: " << size()
            << "\n";
}

