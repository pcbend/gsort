#ifndef __GTYPES_H__
#define __GTYPES_H__

#include<TObject.h>
#include<Rtypes.h>

class Gint;
R__EXTERN Gint *gInt;

enum class EConnectionType {
  kFILE,
  kTHREAD,
  kHIST,
  kVOID,
  kUNKNOWN
  };

enum class EFileType {
  kUNKNOWN          =  -1,
  kDAT              =   2, 
  kGEB              =   3,
  kMID              =   4,
  kEVT              =   5,
  kROOTFILE         = 256,
  kMACRO            = 257,
  kCALIBRATION      = 512,
  kCUTS             = 2052
};


class myType { 
  public:
    myType();
    ~myType();

  ClassDef(myType,0)
};

#endif
