#ifndef __GEBFILE_H__
#define __GEBFILE_H__

#include <GFileInput.h>

class GEBEvent;

class GEBFile : public GFileInput { 
  public: 
    GEBFile(const char *fname);
    ~GEBFile();

  bool Read(GEBEvent &event);
  bool AutoRead();  

  private:
  
};

#endif
