
#include <Gtypes.h>
#include <GBanks.h>
//empty file for cmake....


void Rec::print(const FileContext &ctx) const {
  const char *recStart = ctx.base + offset;
  auto* header = reinterpret_cast<const GEBHeader*>(recStart);
  const char* payload = recStart + sizeof(GEBHeader);
  std::cout << *header << std::endl;
  int counter = 0;
  while(counter<header->size) {
    printf("0x%04x ",*((unsigned short*)(payload+counter)));
    counter+=2;
    if((counter%32)==0)
      printf("\n");
  }
  printf("\n");
}


myType::myType() { } 

myType::~myType() { } 


