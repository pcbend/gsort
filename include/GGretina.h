#ifndef TGRETINA_H
#define TGRETINA_H

#ifndef __CINT__
#include <functional>
#endif

#include "GDetector.h"

#include "Gtypes.h"
#include "GBanks.h"

class GGretina : public GDetector {

public:
  GGretina();
  ~GGretina();

  const GEBBankType1 &GetHit(int i) { return fHits.at(i); } 
  size_t Size() const { return fHits.size(); } 

  void Push(const GEBBankType1 &hit); // { fHits.push_back(hit); } 

  EDetector GetDetector() const override { return EDetector::kGretina; }
  void CopyTo(void* branchObj) const override;
  void Clear() override;

  void Print(Option_t *opt="") const;

private:
  std::vector<GEBBankType1> fHits;

  ClassDefOverride(GGretina,1);
};



#endif
