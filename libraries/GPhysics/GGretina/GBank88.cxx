#include "GBank88.h"

#include "GBanks.h"

GBank88::GBank88() { }

GBank88::~GBank88() = default;

void GBank88::Clear() {
  for(size_t i=0;i<MAX_BANK88_HITS;i++) 
    fChannels[i].Clear();

  //fChannels.clear();
}

int GBank88::Unpack(const Rec &rec,const FileInfo &info) {
  Clear();
  //fChannels.reserve(10);

  const char* sptr = info.base + rec.offset;
  const char* ptr = info.base + rec.offset;
 
  constexpr int GEBHeadSize   = sizeof(GEBHeader);
  constexpr int mode3DataSize = sizeof(GEBMode3Data);
  constexpr int mode3HeadSize = sizeof(GEBMode3Head);

  auto* header = reinterpret_cast<const GEBHeader*>(ptr);
  ptr += GEBHeadSize;

  SetTimestamp(header->timestamp);

  int bytes = GEBHeadSize;
  const uint16_t *words = reinterpret_cast<const uint16_t*>(ptr);
  GMode3Hit hit;
  int fNHits = 0;
  while((bytes+mode3DataSize+mode3HeadSize) < header->size) {
    int bytesRead = hit.Unpack(words,0) - 4;
    words += bytesRead/2;
    bytes+=bytesRead;
    if(fNHits<MAX_BANK88_HITS)
      fChannels[fNHits++] = hit;  
  }
  return 0;
}


void GBank88::CopyTo(void* branchObj) const {
  if(!branchObj) return;
  auto *dst = static_cast<GBank88*>(branchObj);

  dst->fTimestamp = this->fTimestamp;
  for(size_t i=0;i<MAX_BANK88_HITS;i++)
    dst->fChannels[i] = this->fChannels[i];
}


/*
//DEBUG
int GBank88::Unpack(const Rec &rec,const FileInfo &info) {

  rec.print(info);
  const char* sptr = info.base + rec.offset;
  const char* ptr = info.base + rec.offset;
 
  constexpr int GEBHeadSize   = sizeof(GEBHeader);
  constexpr int mode3DataSize = sizeof(GEBMode3Data);
  constexpr int mode3HeadSize = sizeof(GEBMode3Head);

  auto* header = reinterpret_cast<const GEBHeader*>(ptr);
  ptr += GEBHeadSize;

  printf("Data size: %i\n",header->size);
  printf("mode3DataSize: %i\n",mode3DataSize);

  int bytes = GEBHeadSize;
  const uint16_t *words = reinterpret_cast<const uint16_t*>(ptr);
  GMode3Hit hit;
  printf("[ 0] 0x%04x ",*(words+0));
  printf("[+1] 0x%04x ",*(words+1));
  printf("[+2] 0x%04x ",*(words+2));
  printf("\n");
  while((bytes+mode3DataSize+mode3HeadSize) < header->size) {
    int bytesRead = hit.Unpack(words,0) - 4;
    words += bytesRead/2;
    bytes+=bytesRead;
    //words += (hit.Unpack(words,0)/2) - 2;
    
    printf("[-2] 0x%04x ",*(words-2));
    printf("[-1] 0x%04x ",*(words-1));
    printf("[ 0] 0x%04x ",*(words+0));
    if(bytes+4<header->size) {
      printf("[+1] 0x%04x ",*(words+1));
      printf("[+2] 0x%04x ",*(words+2));
    }
    printf(" %li \n", reinterpret_cast<const char*>(words) - sptr );
  }
  printf("\n");
 return 0;
}
*/

void GBank88::Print(Option_t* opt) const {
  printf("GBank88 @ %llu\n",Timestamp());
  for(size_t i=0;i<MAX_BANK88_HITS;i++) {
    printf("\t%zu: ",i);
    fChannels[i].Print();
  }
  printf("-----------------------------\n");
}

