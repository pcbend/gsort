#ifndef _GEBEVENT_H_
#define _GEBEVENT_H_

#include "GEvent.h"
#include "GBanks.h"

class GEBEvent : public GEvent {
public:
  GEBEvent();
  GEBEvent(const GEvent&);
  virtual ~GEBEvent();

  long GetTimestamp() const;

  const char* GetBuffer() const;

  virtual void Clear(Option_t *opt ="");
  virtual void Print(Option_t *opt ="") const;

  int Type() const { return GetHeader().type;  }
  int Size() const { return GetHeader().size;  }

private:
  GEBHeader GetHeader() const { return *((GEBHeader*)&fEventHeader); }


  ClassDef(GEBEvent, 0);
};


/*
class GEBMode3Event : public TObject {
public:
  GEBMode3Event(GEBEvent& event)
    : fEvent(event) {
    BuildFragments();
  }

  GEBEvent& GetFragment(size_t fragnum) { return fragments[fragnum]; }
  size_t     NumFragments() const { return fragments.size(); }

private:
  void BuildFragments();

  GEBEvent fEvent;
  std::vector<GEBEvent> fragments;

  ClassDef(GEBMode3Event,0);
};
*/


#endif /* _GEBEVENT_H_ */
