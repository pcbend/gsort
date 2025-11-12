#include <GEBEvent.h>


GEBEvent::GEBEvent() { } 

GEBEvent::GEBEvent(const GEvent&) { }

GEBEvent::~GEBEvent() { }

long GEBEvent::GetTimestamp() const { return 0; }

const char* GEBEvent::GetBuffer() const { return 0; }

void GEBEvent::Clear(Option_t *opt) { }

void GEBEvent::Print(Option_t *opt) const { }


