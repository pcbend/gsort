
#include <GFileInput.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h> // For mmap() and munmap()
#include <sys/stat.h> // For fstat()

#include <iostream>

#include <GEvent.h>

GFileInput::GFileInput(const char *fname) {

  fFILE  = open(fname, O_RDONLY);
  struct stat sb;
  if(fstat(fFILE,&sb)==-1) 
    printf(" file %s not found...\n",fname);
  
  fFileSize = sb.st_size;
  fName = fname;
  fTitle = "File Input";

  fFileMap = mmap(NULL,fFileSize,PROT_READ,MAP_PRIVATE,fFILE,0);
  if(fFileMap == MAP_FAILED) {
    // handle error.
  }
  fFilePtr = static_cast<char*>(fFileMap);
  
  SetInputConnectionType(EConnectionType::kFILE);

  doStart(); 
} 

GFileInput::~GFileInput() { 
  if(munmap(fFileMap,fFileSize)==-1) {
    //handle error
  }
  close(fFILE);
} 

void GFileInput::SetFileType() {
  fFileType = EFileType::kGEB;
}


void GFileInput::Initialize() { 
  printf("Starting sort of %s\n",fName.c_str());
  printf("\t  %.2f GB\n",fFileSize/1024./1024./1024.);
  
} 

bool GFileInput::Iteration() { 
  //static int count=0;
  //printf(" counting %i...\n",count++);
  //fFile.rdbuf()->pupsetbuf(fBuffer,BUFFERSIZE);
  //std::this_thread::sleep_for(std::chrono::seconds(100));
  //GEvent *event;
  //bool state = Read(event);
  bool state = AutoRead();
  if(state) {
    //push event.
  }

  return state;
}

void GFileInput::Finalize() { }


bool GFileInput::Read(GEvent &event) {
  event.fData = fFilePtr;
  event.fEventHeader = *((RawHeader*)fFilePtr);
  fFilePtr+=sizeof(event.fEventHeader);
  std::cout << event.fEventHeader << std::endl;

  return true;
}

