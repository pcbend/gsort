#ifndef GMODE3HIT_H
#define GMODE3HIT_H

#include <GDetectorHit.h>

#define MAXTRACE 1024

class GMode3Hit : public GDetectorHit {
public:
  GMode3Hit();
  ~GMode3Hit();

  int Unpack(const uint16_t *words,int16_t size);

  //virtual size_t  Size()  const { return waveform.size();     }
  //Int_t WaveSize()        const { return wavesize; }
  const std::vector<int16_t>& GetWave() const { return waveform; }

  uint16_t    GetChannel()   const { return (board_id & 0x000f);     }
  uint16_t    GetVME()       const { return (board_id & 0x0030)>>4;    }
  uint16_t    GetCrystal()   const { return (board_id & 0x00c0)>>6;    }
  uint16_t    GetHole()      const { return (board_id & 0x1f00)>>8;        }
  uint16_t    GetSegmentId() const { return GetVME()*10 + GetChannel(); }
  uint16_t    GetAbsSegId()  const { return GetCrystal()*40 + GetSegmentId() ; }
  uint16_t    GetCrystalId() const { return GetHole()*4 + GetCrystal(); }

  uint64_t    GetLed()       const { return Timestamp(); }
  int64_t     GetCfd()       const { return cfd; }

  uint16_t    GetDt1()       const { return dt1; }
  uint16_t    GetDt2()       const { return dt2; }

  float       GetCharge()    const { return charge;  }
  int32_t     GetCharge0()   const { return charge0; }
  int32_t     GetCharge1()   const { return charge1; }
  int32_t     GetCharge2()   const { return charge2; }

  //static void SetExtractWaves(bool flag=true) { fExtractWaves = flag;  }
  //static bool ExtractWaves()                  { return fExtractWaves;  }

  //virtual Int_t  Charge() const   { return TDetectorHit::Charge()/128.; }

  //double GetEnergy0() const;
  //double GetEnergy1() const;
  //double GetEnergy2() const;

  bool operator<(const GMode3Hit &rhs) const { return GetSegmentId() < rhs.GetSegmentId(); }

  void Print(Option_t *opt="") const;
  void Clear();

private:
  uint16_t  board_id;
  //uint16_t  wavesize; // In 16-bit elements
  //int64_t   led;  //Timestamp
  int64_t   cfd;
  uint16_t dt1;    // time diff between this led and previous (last) led
  uint16_t dt2;    // time diff between previous and previous-previouss (yep.) led

  float    charge;   // 
  int32_t  charge0;  // charge using pick-off method.
  int32_t  charge1;  // charge of previous.
  int32_t  charge2;  // charge of previous-previous.
  
  std::vector<int16_t> waveform;

  ClassDef(GMode3Hit,1);
};





#endif
