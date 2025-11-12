#ifndef __GFILE_H__
#define __GFILE_H__

#include<string>

#include<TObject.h>

class GFile : public TObject {
  public:
    GFile(std::string inFile);
    ~GFile();


  void ReadSome();
  void Read();

  enum EFileType {
    kGEB,
    kEVT 
  };


  private:
    //bookkeeping
    void SetFileType(std::string&);
    EFileType fFileType;
    
    //mmapping
    int    fFd;
    off_t  fSize;
    off_t  fOffset;
    void*  fMapFile;
    bool   fMapOpened;
    char*  fData;

  ClassDef(GFile,0)
};

#endif

