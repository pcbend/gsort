#ifndef _GDETECTORHIT_H_
#define _GDETECTORHIT_H_

#include <TObject.h>

class GDetectorHit {
public:
  GDetectorHit();
  virtual ~GDetectorHit();

  //virtual const char* GetName() const;
  //virtual int         GetNumber() const;

  //virtual Int_t Compare(const TObject *obj) const; //needed for root containers
  //virtual bool IsSortable() const { return true; }

  //virtual void  Copy(TObject&) const;
  //virtual void  Clear(Option_t *opt = "" );
  //virtual void  Print(Option_t *opt = "" ) const;

  void SetAddress(uint32_t address)      { fAddress = address; }
  uint32_t  Address()   const            { return fAddress; }
  
  void SetTimestamp(uint64_t timestamp)  { fTimestamp = timestamp; }
  uint64_t Timestamp() const             { return fTimestamp; }


  //virtual Int_t Charge() const;
  //virtual float RawCharge() const { return fCharge; }
  //Int_t  Time() const             { return fTime; }

  //void SetCharge(int charge);
  //void SetTime(int time)            { fTime = time; }

  //double GetEnergy() const; //applies TChannel ENERGYCOEFF to Charge
  //void SetEnergy(double energy);
  //double GetTime() const;   //applies TChannel TIMECOEFF to Time()

  //void AddEnergy(double eng);

  //static const TVector3 BeamUnitVec; //!

protected:
  uint32_t fAddress;
  uint64_t fTimestamp;
  //float    fCharge;
  //double   fTime;

  ClassDef(GDetectorHit,2)
};

#endif
