
#include <GEBFile.h>

#include <GBanks.h>
#include <GEBEvent.h>

GEBFile::GEBFile(const char *fname) : GFileInput(fname) {  }

GEBFile::~GEBFile() { 
  GFileInput::~GFileInput();

}


bool GEBFile::Read(GEBEvent& event) { 
  GFileInput::Read(event); 
  //event.GetHeader();

  return true; 
} 

bool GEBFile::AutoRead() { 

  GEvent *event;

  std::this_thread::sleep_for(std::chrono::seconds(100));

  return true; 

} 
