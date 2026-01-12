
#include <Unpacker.h>
#include <GDetector.h>

#include <Gtypes.h>
#include <GBanks.h>

#include <GBank88.h>

#include <GHistogramer.h>

std::vector<std::unique_ptr<GDetector> > Unpack(const std::vector<Rec> &event, const FileInfo &info) {
  switch(info.type) {
    case EFileType::kGEB:
      return UnpackGEBTYPE1(event,info);
    case EFileType::kEVT:
     return  UnpackEVT(event,info);
    case EFileType::kMID:
      return UnpackMID(event,info);
    default:
      printf("unpacker - unknown type: %i\n",int(info.type));
      break;
  };
  return {};
}

std::vector<std::unique_ptr<GDetector> > UnpackGEBTYPE1(const std::vector<Rec> &event, const FileInfo &info) {
  std::vector<std::unique_ptr<GDetector> > out;
  out.reserve(2);
  for(auto r : event) {
    const char *recStart = info.base + r.offset;
    auto* header = reinterpret_cast<const GEBHeader*>(recStart);
    const char* payload = recStart + sizeof(GEBHeader);
    switch(header->type) {
      case 1: //Gretina Decomp data. 
        {
          auto *hit = reinterpret_cast<const GEBBankType1*>(payload); 
          GHistogramer::Get().Fill("summary",16000,0,8000,hit->tot_e,200,0,200,hit->crystal_id);
        }
        break;
      case 8: //bank88 
        {
          auto bank88 = std::make_unique<GBank88>();
          bank88->Unpack(r,info);
          out.emplace_back(std::move(bank88));
        }
        break;
      default:
        break;
    };

  }
  return out;
}

std::vector<std::unique_ptr<GDetector> > UnpackEVT(const std::vector<Rec> &event, const FileInfo &info) {
  std::vector<std::unique_ptr<GDetector> > out;
  printf("evt unpack not implemented...\n");
  return out;
}

std::vector<std::unique_ptr<GDetector> > UnpackMID(const std::vector<Rec> &event, const FileInfo &info) {
  std::vector<std::unique_ptr<GDetector> > out;
  printf("mid unpack not implemented...\n");
  return out;
}

