
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
  
  GThread<Rec>::Register();  
  
  fMapFile = mmap(NULL,fSize,PROT_READ,MAP_PRIVATE,fFd,0);
  if(!(fMapFile == MAP_FAILED)) {
    fMapOpened = true;
    fData = static_cast<char*>(fMapFile);//
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
  if(fOffset>=fSize) return false;

  GEBHeader *header = (GEBHeader*)(fData+fOffset);  //most recent...
  
  static uint64_t seq=0;
  this->emplace(Rec{static_cast<uint64_t>(header->timestamp),
                    static_cast<uint32_t>(header->type),
                    static_cast<uint32_t>(header->size),
                    static_cast<uint64_t>(fOffset),
                    seq++});
  fOffset += sizeof(GEBHeader) + header->size;
  
  return true; 

  Rec top;
  for(;;) {
   if(!peek(top)) break;
   uint64_t oldest = top.timestamp;
   if(header->timestamp<oldest) break;
   if(header->timestamp-oldest < 3'000'000'000'000ull) break; //30'000s @ 10ns
   printf("gfile iteration is blocking!\n"); fflush(stdout);
   printf("\t header: %lld \n",header->timestamp);
   printf("\t oldest: %lld \n",oldest); 

   std::this_thread::sleep_for(std::chrono::milliseconds(100));  
  }

  return true; 
} 



GEventBuilder::GEventBuilder(GFile *ptr) { } 

GEventBuilder::~GEventBuilder() { } 

bool GEventBuilder::Iteration() {

  return true;
}




