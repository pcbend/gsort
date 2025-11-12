#ifndef __GFILE_H__
#define __GFILE_H__

#include<string>

#include<TObject.h>
#include<GThread.h>


struct Rec {
  uint64_t timestamp;
  uint32_t type;
  uint32_t size;
  uint64_t offset;
  uint32_t seq;
  bool operator<(const Rec& other) const { 
    if(timestamp != other.timestamp) 
      return timestamp<other.timestamp; 
    return seq<other.seq; 
  }
};

class GFile : public GThread<Rec> {
  public:
    GFile(std::string inFile);
    virtual ~GFile();


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

