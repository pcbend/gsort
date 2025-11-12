#ifndef __GEVENT_H__
#define __GEVENT_H__

#include <TObject.h>

#include <GBanks.h>
#include <Gtypes.h>

class GEvent : public TObject {
  public:
    GEvent();
    GEvent(const GEvent&);
    ~GEvent();

    void Copy(GEvent &other) const {  } 

    char *GetDataPtr() { return fData; }
  
  //private: 
    char *fData; // can this just point to the approptiate spot in the mmap?
    //std::shared_ptr<char> fBuffer;i
    RawHeader fEventHeader;
    EFileType fFileType;
    long      fTimestamp; 


  ClassDef(GEvent,0);
};

#endif
