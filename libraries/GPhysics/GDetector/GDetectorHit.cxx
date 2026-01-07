#include "GDetectorHit.h"

#include <iostream>
#include <cmath>

#include "TClass.h"
#include "TRandom.h"
#include "TBuffer.h"

GDetectorHit::GDetectorHit() {
  //Class()->CanIgnoreTObjectStreamer();
  Clear();
}

GDetectorHit::~GDetectorHit() { }

void GDetectorHit::Clear(Option_t *opt) {
  TObject::Clear(opt);
  fAddress   = 0;
  fCharge    = 0;
  fTime      = 0;
  fTimestamp = 0;
}
void GDetectorHit::Print(Option_t *opt) const { }

void GDetectorHit::Copy(TObject& obj) const {
  TObject::Copy(obj);

  GDetectorHit& hit = (GDetectorHit&)obj;
  hit.fAddress = fAddress;
  hit.fCharge = fCharge;
  hit.fTime = fTime;
  hit.fTimestamp = fTimestamp;
}


/*
void GDetectorHit::SetCharge(int charge) {
  fCharge = charge + gRandom->Uniform();
  fFlags &= ~kIsEnergy;
}

void GDetectorHit::SetEnergy(double energy) {
  fCharge = energy;
  fFlags |= kIsEnergy;
}
*/

/*
Int_t  GDetectorHit::Charge() const {
  if(fFlags & kIsEnergy) {
    return 0;
  } else {
    return RawCharge();
  }
}
*/

/*
double GDetectorHit::GetEnergy() const {
  if(fFlags & kIsEnergy) {
    return RawCharge();
  } else {
    TChannel* chan = TChannel::GetChannel(fAddress);
    if(!chan){
      return fCharge;
    } else {
      return chan->CalEnergy(RawCharge(), fTimestamp);
    }
  }
}

void GDetectorHit::AddEnergy(double eng) {
  SetEnergy(eng + GetEnergy());
}
*/



//double GDetectorHit::GetTime() const { return fTime; }
  
//  TChannel* chan = TChannel::GetChannel(fAddress);
//  if(!chan){
//    return Time() + gRandom->Uniform();
//  }
//  return chan->CalTime(Time(), fTimestamp);
//}

/*
Int_t GDetectorHit::Compare(const TObject *obj) const {
  const GDetectorHit* other = (const GDetectorHit*)obj;
  if(GetEnergy() < other->GetEnergy()) {
    return -1;
  } else if (GetEnergy() > other->GetEnergy()) {
    return +1;
  } else {
    return 0;
  }
}
*/
