#ifndef __GFILE_H__
#define __GFILE_H__

#include<string>
#include<limits>

#include<TObject.h>
#include<GThread.h>



struct Rec {
  uint64_t timestamp;
  uint32_t type;
  uint32_t size;
  uint64_t offset;
  uint64_t seq;
  bool operator<(const Rec& other) const { 
    if(timestamp != other.timestamp) 
      return timestamp>other.timestamp; 
    return seq<other.seq; 
  }
};

class GFile : public GThread<Rec> {
  public:
    GFile(std::string inFile);
    virtual ~GFile();

    // DELETE these to prevent accidental memory corruption
    GFile(const GFile&) = delete;
    GFile& operator=(const GFile&) = delete;

    void ReadSome();
    void Read();
    void Sort(); 

    enum EFileType {
      kGEB,
      kEVT 
    };

  protected:
    bool Iteration() override; 

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

    //Queue information

  ClassDef(GFile,0)
};

class GEventBuilder : public GThread<std::vector<Rec> > {
  public:
    GEventBuilder(GFile *ptr);
    virtual ~GEventBuilder(); 

  protected:
    bool Iteration() override; 


  ClassDef(GEventBuilder,0) 
};


#endif

