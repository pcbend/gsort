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
  int64_t operator-(const Rec& other) const {
    return static_cast<int64_t>(timestamp) - static_cast<int64_t>(other.timestamp);
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

    bool pop(Rec &rec) override;

    void print() override;

    enum EFileType {
      kGEB,
      kEVT 
    };

  protected:
    bool Iteration() override; 

  private:
    //bookkeeping
    bool fDoneReading{false};
    uint64_t fLastTimestamp{0};
    
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



#endif

