#ifndef _GDETECTORHIT_H_
#define _GDETECTORHIT_H_

#include <TObject.h>

class GDetectorHit : public TObject {
public:
  GDetectorHit();
  virtual ~GDetectorHit();

  //virtual const char* GetName() const;
  //virtual int         GetNumber() const;

  //virtual Int_t Compare(const TObject *obj) const; //needed for root containers
  //virtual bool IsSortable() const { return true; }

  virtual void  Copy(TObject&) const;
  virtual void  Clear(Option_t *opt = "" );
  virtual void  Print(Option_t *opt = "" ) const;

  Int_t  Address()   const        { return fAddress; }
  //virtual Int_t Charge() const;
  //virtual float RawCharge() const { return fCharge; }
  //Int_t  Time() const             { return fTime; }
  long   Timestamp() const        { return fTimestamp; }

  void SetAddress(int address)      { fAddress = address; }
  //void SetCharge(int charge);
  //void SetTime(int time)            { fTime = time; }
  void SetTimestamp(long timestamp) { fTimestamp = timestamp; }

  //double GetEnergy() const; //applies TChannel ENERGYCOEFF to Charge
  //void SetEnergy(double energy);
  //double GetTime() const;   //applies TChannel TIMECOEFF to Time()

  //void AddEnergy(double eng);

  //static const TVector3 BeamUnitVec; //!

protected:
  uint32_t fAddress;
  float    fCharge;
  uint64_t fTimestamp;
  double   fTime;

  ClassDef(GDetectorHit,1)
};

#endif
