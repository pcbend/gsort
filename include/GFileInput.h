#ifndef __GFILEINPUT_H__
#define __GFILEINPUT_H__

#include<GThread.h>

#include <string>
//#include <fstream>

#include <Gtypes.h>

class GEvent;

//#define BUFFERSIZE 16384

class GFileInput : public GThread {
  public:
    GFileInput(const char *fname);
    ~GFileInput();

    void Initialize() override;
    bool Iteration()  override;
    void Finalize()   override;

    EFileType GetFileType() const { return fFileType; }

    virtual bool Read(GEvent &event);

  private:
    void SetFileType(); 
    virtual bool AutoRead()        = 0;

  private:    
    EFileType     fFileType;

    std::string   fName;
    std::string   fTitle;
    //std::ifstream fFile; 
    int           fFILE;
    void*         fFileMap;
    size_t        fFileSize;

    char    *fFilePtr;
    //size_t         fOffset; //?

  ClassDefOverride(GFileInput,0)
};

#endif

