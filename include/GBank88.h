#ifndef TBANK88_H
#define TBANK88_H

#include "GDetector.h"
#include "GMode3Hit.h"

#include "Gtypes.h"

#define MAX_BANK88_HITS 10

class GBank88 : public GDetector {

  public:
    GBank88();
    ~GBank88();

    //void PrintHit(int i){ gretina_hits->At(i)->Print(); }
    //uint64_t Timestamp() { return Timestamp(); }
    //if(Size()>0) return ((TMode3Hit&)GetHit(0)).GetTimestamp(); else return -1; }


    const GMode3Hit &GetHit(int i) const { return fChannels.at(i); }
    size_t Size() const { return fChannels.size(); } 

    int Unpack(const Rec &rec,const FileInfo &info);  

    EDetector GetDetector()      const override { return EDetector::kBank88; } 
    void CopyTo(void* branchObj) const override;
    void Clear() override;

    void Print(Option_t* opt="") const;

  private:
    std::array<GMode3Hit,MAX_BANK88_HITS> fChannels;
  
  ClassDefOverride(GBank88,1);
};





#endif
