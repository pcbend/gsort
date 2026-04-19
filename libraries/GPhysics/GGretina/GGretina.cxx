
#include "GGretina.h"

#include "GBanks.h"

GGretina::GGretina() { } 

GGretina::~GGretina() { } 

void GGretina::Clear() {
  GDetector::SetTimestamp(0);
  fHits.clear();
}

void GGretina::Push(const GEBBankType1 &hit) { 
  if(!fHits.size())
    this->SetTimestamp(hit.timestamp);
  fHits.push_back(hit); 
} 

void GGretina::CopyTo(void* branchObj) const { 
  if(!branchObj) return;
  auto *dst = static_cast<GGretina*>(branchObj);

  dst->fTimestamp = this->fTimestamp;
  for(size_t i=0;i<fHits.size();i++) 
    dst->fHits.push_back(fHits[i]);


} 

void GGretina::Print(Option_t *opt) const { } 




