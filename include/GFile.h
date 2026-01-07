#ifndef __GFILE_H__
#define __GFILE_H__

#include<string>
#include<limits>

#include<TObject.h>
#include<GThread.h>

#include <Gtypes.h>

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

    const FileContext &Context() const { return fCtx; }

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
    FileContext fCtx;

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

