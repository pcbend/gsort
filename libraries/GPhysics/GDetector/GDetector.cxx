#include <GDetector.h>
#include <GBanks.h>
#include <GHistogramer.h>

#include <vector>

GDetector::GDetector() {
  //Class()->CanIgnoreTObjectStreamer();
  //Clear();
}

//GDetector::GDetector(const char* name,const char* title) : TNamed(name,title) {
  //Clear();
  //Class()->CanIgnoreTObjectStreamer();
//}

//GDetector::~GDetector() { }

//void GDetector::Clear(Option_t *opt) {
  //TNamed::Clear(opt);
//  fTimestamp = -1;
//}

//void GDetector::Print(Option_t *opt) const { }

//void GDetector::Copy(TObject& obj) const {
//  TNamed::Copy(obj);

//  GDetector& det = (GDetector&)obj;
//  det.fTimestamp = fTimestamp;
//}

//int GDetector::Compare(const TObject& obj) const {
//  GDetector& det = (GDetector&)obj;
//  if(fTimestamp < det.fTimestamp) {
//    return -1;
//  } else if (fTimestamp > det.fTimestamp) {
//    return +1;
//  } else {
//    return 0;
//  }
//}

/*

int GDetector::Unpack(std::vector<Rec> &event,const FileInfo &ctx) { 
  for(auto r : event) {
    const char *recStart = ctx.base + r.offset;
    auto* header = reinterpret_cast<const GEBHeader*>(recStart);
    const char* payload = recStart + sizeof(GEBHeader);
    //std::cout << *header << std::endl;
    if(header->type==1)  {
      auto *hit = reinterpret_cast<const GEBBankType1*>(payload);
      //GHistogramer::Get().Fill(Form("xtal%03i",hit->crystal_id),16000,0,8000,hit->tot_e);
      GHistogramer::Get().Fill("summary",16000,0,8000,hit->tot_e,200,0,200,hit->crystal_id);
    } else if (header->type==8) {
      //r.print(ctx);

      int size=0;
      const char *ptr = payload;
      const char *payloadEnd = payload + header->size;
  
      while(ptr + 4*sizeof(UShort_t) <= payloadEnd) {
        auto const *words = reinterpret_cast<const UShort_t*>(ptr);

        if(words[0] != 0xaaaa || words[1] != 0xaaaa) { break; }
        GEBMode3Head head3{};
        head3.a1       = words[0];
        head3.a2       = words[1];
        head3.lengthGA = words[2];
        head3.board_id = words[3];

        head3 = SwapMode3Head(head3);

        const int packetBytes = head3.GetLength() - 2*sizeof(UShort_t);

        if (packetBytes <= 0 || ptr + packetBytes > payloadEnd) { break; }
        
        constexpr int headerWords = 4;               // aaaa aaaa lengthGA board_id
        constexpr int headerBytes = headerWords * sizeof(UShort_t);
        const char *dataPtr    = ptr + headerBytes;
        const size_t dataBytes = packetBytes - headerBytes;

        const auto *rawSamples = reinterpret_cast<const GEBMode3Data*>(dataPtr);
        const size_t nSamples  = dataBytes / sizeof(GEBMode3Data);
        for (size_t i = 0; i < nSamples; ++i) {
          GEBMode3Data sample = rawSamples[i];  // copy so we can byte-swap
          sample = SwapMode3Data(sample);

          //std::cout << head3 << sample << "\n";
        }
        ptr += packetBytes;
      }
    }
  }  //continue;
  //printf("----------------------\n");

  return 0;
}

*/

//int GDetector::Build(std::vector<TRawEvent>& raw_data){
//  int output = 0;
//  return output;
//}





