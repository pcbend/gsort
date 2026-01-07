#ifndef _GDETECTOR_H_
#define _GDETECTOR_H_

#include <vector>

#include <Gtypes.h>

#include <TNamed.h>
#include <GDetectorHit.h>

class GDetector : public TNamed {
  public:
    GDetector();
    GDetector(const char *name,const char *title="");
    virtual ~GDetector();

    virtual void Copy(TObject& obj) const;
    virtual void Clear(Option_t *opt = "" );
    virtual void Print(Option_t *opt = "" ) const;
    virtual int  Compare(const TObject& obj) const;

    //int Build(std::vector<TRawEvent>& raw_data);
    //virtual void InsertHit(const GDetectorHit&) = 0;
    //virtual GDetectorHit& GetHit(int i)         = 0;

    //virtual size_t Size() const = 0; //{ return (size_t)fSize; }

    Long_t Timestamp() const { return fTimestamp; }
    void   SetTimestamp(Long_t timestamp)  { fTimestamp = timestamp; }

    //enum EDetectorStatus { kUnbuilt = BIT(15) };
  
    //unsigned int RunStart() const { return fRunStart; }
    //virtual void SetRunStart(unsigned int unix_time) { fRunStart = unix_time; }
    //int Build(); // build from transient data member.

    inline bool operator<(const GDetector& other) const {
      // reverse comparison so smallest timestamp has highest priority
      return fTimestamp > other.fTimestamp;
    }

    //int Unpack(std::vector<Rec> &event);
    int Unpack(std::vector<Rec> &event,const FileContext &ctx);

  protected:
    Long_t fTimestamp;

  private:
    //std::vector<Rec*> fRawData;  //!
    std::vector<GDetectorHit> fHits;


  ClassDef(GDetector,1)
};

#endif
