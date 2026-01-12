#include "GMode3Hit.h"
//#include "TRandom.h"

#include "GBanks.h"

//bool GMode3Hit::fExtractWaves = true;

GMode3Hit::GMode3Hit() { }

GMode3Hit::~GMode3Hit() { }

int GMode3Hit::Unpack(const uint16_t *words,int16_t size) {
  if(words[0] != 0xaaaa || words[1] != 0xaaaa) { return -1; }
  GEBMode3Head head3{};           
  //head3.a1       = words[0];      
  //head3.a2       = words[1];      
  head3.lengthGA = words[2];      
  head3.board_id = words[3];      
  head3 = SwapMode3Head(head3);

  GEBMode3Data data3 = *(reinterpret_cast<const GEBMode3Data*>(words+4));
  data3 = SwapMode3Data(data3);
  
  size_t wave_bytes = head3.GetLength()*4 - sizeof(head3) + 4 - sizeof(data3);  
/*
  if(read_waveform){
    size_t wavesize = wave_bytes/sizeof(short);

    //std::cout << "wave size:  " << wavesize << std::endl;
    waveform.resize(wavesize);

    memcpy((char*)&waveform[0], buf.GetData(), wave_bytes);

    for(unsigned int i=0; i<wavesize; i+=2){
      short tmp      = TRawEvent::SwapShort(waveform[i+1]);
      waveform[i+1] = TRawEvent::SwapShort(waveform[i]);
      waveform[i]   = tmp;
    }
  }
*/

//board_id = head3.board_id;
  SetAddress(head3.board_id);
  SetTimestamp(data3.GetLed());
  charge = data3.GetEnergy(head3);
  
  dt1  = data3.GetDeltaT1();
  dt2  = data3.GetDeltaT2();

  charge0  = data3.GetEnergy0(head3);
  charge1  = data3.GetEnergy1(head3);
  charge2  = data3.GetEnergy2(head3);

  cfd = data3.GetCfd();

  return head3.GetLength();  // bytes of the event?

  //0x30 0011 0000 

  //head3.GetLength();
}

void GMode3Hit::Print(Option_t *opt) const {
  printf("board[0x%04x] xtal[%i] Channel[%i] Charge[%.1f] @ %llu\n",board_id,GetCrystal(),GetChannel(),GetCharge(),Timestamp());
  //printf("\tboard: \t0x%04x",board_id);
  //printf("\tcharge:\t%.1f\n",GetCharge());
  //printf("\tcharge0:\t%i\n",GetCharge0());
  //printf("\tcharge1:\t%i\n",GetCharge1());
  //printf("\tcharge2:\t%i\n",GetCharge2());
}

void GMode3Hit::Clear() {
  board_id = 0;
  cfd      = 0;
  dt1      = 0;
  dt2      = 0;
  charge   = 0;
  charge0  = 0;
  charge1  = 0;
  charge2  = 0;
  waveform.clear();
}

