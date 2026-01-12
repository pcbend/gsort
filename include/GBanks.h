#ifndef GBANKS__H
#define GBANKS__H

//#include <cstdio>
#include <iostream>
#include <iomanip>

//#include <stdint.h>
#include <Rtypes.h>

//  https:://gswg.lbl.gov/tiki-index.php?page=GEB+Headers

#define MAXSIMSIZE 40
#define MAX_INTPTS 16
#define MAX_PWID   256
#define MAX_LABRID 16
#define MQDC_ID 161
#define MTDC_ID 177

// Simulation
#define MAX_SIM_GAMMAS 10

struct g4sim_ghead{
  int type;          /* defined as abcd1234 */
  int num;           /* # of emitted gammas */
  int full;          /* is full energy */

  int GetType() const { return type; }
  int GetNum()  const { return num; }
  int GetFull() const { return full; }
}__attribute__((__packed__));

struct g4sim_eg{
  float e;
  float x, y, z;
  float phi, theta; //THIS IS REDUNDANT
  float beta;

  float GetEn()       const { return e; }
  float GetX()        const { return x; }
  float GetY()        const { return y; }
  float GetZ()        const { return z; }
  float GetPhi()      const { return phi; }
  float GetTheta()    const { return theta; }
  float GetBeta()     const { return beta; }

}__attribute__((__packed__));


struct G4SimPacket {
  g4sim_ghead head;
  g4sim_eg    data[MAXSIMSIZE];
  friend std::ostream& operator<<(std::ostream& os, const G4SimPacket &packet);
}__attribute__((__packed__));


// -- End of Geant4 Gamma Sim stuff
// -- Beginning of Geant4 S800 Sim stuff

struct G4S800 {


  Int_t type;    /* defined abcd1234 for indicating this version */
  // All of this vvv is zero in the sim. ***************************
  float crdc1_x;   /* Crdc x/y positions in mm */
  float crdc1_y;
  float crdc2_x;
  float crdc2_y;
  float ic_sum;    /* ion chamber energy loss         */
  float tof_xfp;   /* TOF scintillator after A1900    */
  float tof_obj;   /* TOF scintillator in object box  */
  float rf;        /* Cyclotron RF for TOF            */
  Int_t trigger; /* Trigger register bit pattern    */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* from here corrected values extracted from data above */
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  float ic_de;
  /* TOF values with TOF correction applied (from afp/crdc x) */
  float tof_xfpe1;
  float tof_obje1;
  float tof_rfe1;
  // All of this ^^^ is zero in the sim. ***************************


  /* Trajectory information at target position calculated from
     a map and afp/bfp/xfp/yfp. New map and you need to re-calc */
  float ata; /* dispersive angle        */
  float bta; /* non-dispersive angle    */
  float dta; /* dT/T T:kinetic energy   */
  float yta; /* non-dispersive position */

  Int_t GetType() const { return type; }
  float GetATA()  const { return ata; }
  float GetBTA()  const { return bta; }
  float GetDTA()  const { return dta; }
  float GetYTA()  const { return yta; }
  friend std::ostream& operator<<(std::ostream& os, const G4S800 &s800pack);
}__attribute__((__packed__));



struct MQDCHeader {
  UInt_t datum;
  UInt_t signature() const { return (datum&0xc0000000)>>30; }
  UInt_t subheader() const { return (datum&0x3f000000)>>24; }
  UInt_t moduleid()  const { return (datum&0x00ff0000)>>16; }
  UInt_t outformat() const { return (datum&0x00008000)>>15; }
  UInt_t adcres()    const { return (datum&0x00007000)>>12; }
  UInt_t size()      const { return (datum&0x00000fff)>>0;  }
};  

struct MTDCHeader {
  UInt_t datum;
  UInt_t signature() const { return (datum&0xc0000000)>>30; }
  UInt_t subheader() const { return (datum&0x3f000000)>>24; }
  UInt_t moduleid()  const { return (datum&0x00ff0000)>>16; }
  UInt_t tdcres()    const { return (datum&0x0000f000)>>12; }
  UInt_t size()      const { return (datum&0x00000fff)>>0;  }
};  





// General Mesytec Stuff:
struct Mesy_Word{
  // This is 2x16 bits = 32 bits.  The MQDC talks in 32 bit words.
  UShort_t tail;
  UShort_t nose;

  bool isHeader() const { return((nose&0xff00)==0x4000); }
  bool isData()   const { return((nose&0xffc0)==0x0400); } // for QDC 0x0020 should also be 0.  Left out for easy use.
  bool isETS()    const { return((nose&0xffff)==0x0480); }
  bool isFILL()   const { return(((nose&0xffff)==0) && ((tail&0xffff)==0)); }
  bool isALLF()   const { return(((nose&0xffff)==0xffff) && ((tail&0xffff)==0xffff)); }
  bool isEOE()    const { return(((nose&0xc000)==0xc000) && !(isALLF())); }
}__attribute__((__packed__));

struct Mesy_Header{
  UShort_t tail;
  UShort_t nose;

  UShort_t format()   const { return ((tail&0x8000)>>15); }
  UShort_t size()     const { return tail&0x0fff; }
  UShort_t id()       const { return nose&0x00ff; }
  UShort_t res()      const { return (tail&0x7000)>>12; }
  bool isQDC()        const { return(id()==MQDC_ID); }
  bool isTDC()        const { return(id()==MTDC_ID); }
}__attribute__((__packed__));

struct Mesy_ETS{
  UShort_t tail;
  UShort_t nose;

  UShort_t ETS()    const { return tail&0xffff; }
}__attribute__((__packed__));

struct Mesy_FILL{
  UShort_t tail;
  UShort_t nose;
}__attribute__((__packed__));

struct Mesy_EOE{
  UInt_t data;

  Int_t TS()     const { return data&0x3fffffff; }
}__attribute__((__packed__));

// Mesytec QDC:
struct M_QDC_Data{
  UShort_t tail;
  UShort_t nose;

  UShort_t Chan()   const { return nose&0x001f; }
  UShort_t Charge() const { return tail&0x0fff; }
  UShort_t isOOR()  const { return (tail&0x4000)>>14; }
}__attribute__((__packed__));

// Mesytec TDC:
struct M_TDC_Data{
  UShort_t tail;
  UShort_t nose;

  bool isTrig() const { return((nose&0x0020)>>5); }
  UShort_t Chan()   const { return nose&0x001f; }
  UShort_t Time()   const { return tail&0xffff; }
}__attribute__((__packed__));




typedef struct CAESARHeader {
  Int_t   totalsize;
  Short_t moresize;
  Short_t tag;
  Short_t version;
  Short_t ulm;
}__attribute__((__packed__)) CAESARHeader;

typedef struct CAESARFeraHeader {
  Short_t size; // Inclusive number of 16-bit values
  Short_t tag;
}__attribute__((__packed__)) CAESARFeraHeader;

typedef struct CAESARFera {
  Short_t header;
  Short_t number_chans() { return  ((header & 0x7800)>>11); }
  Short_t vsn()          { return   (header & 0x00ff)-1; }
//  CAESARFeraItem items[16];

}__attribute__((__packed__)) CAESARFera;


typedef struct CAESARFeraItem {
  Short_t data;
  Short_t channel()  { return  ((data & 0x7800)>>11); }
  Short_t value()    { return   (data & 0x07ff); }
}__attribute__((__packed__)) CAESARFeraItem;



typedef struct GEBInteractionPoint { // HPGe Segment Hit Type 1;
   Float_t     x;
   Float_t     y;
   Float_t     z;
   Float_t     e;        //fraction of the energy
   Int_t       seg;
   Float_t     seg_ener; //energy (cahrge) collected in the segment
  friend std::ostream& operator<<(std::ostream& os, const GEBInteractionPoint &fSeg);
}__attribute__((__packed__)) GEBInteractionPoint;

inline std::ostream& operator<<(std::ostream& os, const GEBInteractionPoint &fSeg) {
   std::streamsize ss = std::cout.precision();
   return os << "HPGeSegment[" << std::setw(3) << fSeg.seg << "]\t("
             << std::setprecision(2)  << fSeg.x << ", " << fSeg.y << ", " << fSeg.z << ")\t"
             << fSeg.seg_ener << " / " << fSeg.e << std::setprecision(ss) << std::endl;
}


typedef struct GEBBankType1 { // Decomposed GRETINA Data
   Int_t     type;
   Int_t     crystal_id;
   Int_t     num;
   Float_t   tot_e;
   Int_t     core_e[4];
   Long_t    timestamp;
   Long_t    trig_time;  
   Float_t   t0;
   Float_t   cfd;
   Float_t   chisq;
   Float_t   norm_chisq;
   Float_t   baseline;
   Float_t   prestep;
   Float_t   poststep;
   Int_t     pad;
   GEBInteractionPoint intpts[MAX_INTPTS];
  friend std::ostream& operator<<(std::ostream& os, const GEBBankType1 &bank);
}__attribute__((__packed__)) GEBBankType1;

inline std::ostream& operator<<(std::ostream& os, const GEBBankType1 &bank) {
   std::streamsize ss = std::cout.precision();
   //os << "************************" << std::endl;
   os << "type        = "<< std::setw(8) << std::hex << bank.type << std::dec << std::endl;
   os << "crystal_id  = "<< std::setw(8) << bank.crystal_id << std::endl;
   os << "num of pts  = "<< std::setw(8) << bank.num << std::endl;
   os << "CC energy   = "<< std::setw(8) << bank.tot_e << std::endl;
   os << "raw CC charge x4: " << std::endl;
   os << "\t[0]: " << std::setw(10) << bank.core_e[0] << std::endl;
   os << "\t[1]: " << std::setw(10) << bank.core_e[1] << std::endl;
   os << "\t[2]: " << std::setw(10) << bank.core_e[2] << std::endl;
   os << "\t[3]: " << std::setw(10) << bank.core_e[3] << std::endl;
   os << "timestamp   = " << bank.timestamp << std::endl;
   os << std::setprecision(2);
   os << "trig_time   = "<< std::setw(8) << bank.trig_time << std::endl;
   os << "t0          = "<< std::setw(8) << bank.t0 << std::endl;
   os << "cfd         = "<< std::setw(8) << bank.cfd << std::endl;
   os << "chisq       = "<< std::setw(8) << bank.chisq << std::endl;
   os << "norm_chisq  = "<< std::setw(8) << bank.norm_chisq << std::endl;
   os << "baseline    = "<< std::setw(8) << bank.baseline << std::endl;
   os << "bl prestep  = "<< std::setw(8) << bank.prestep  << std::endl;
   os << "bl poststep = "<< std::setw(8) << bank.poststep << std::endl;
   os << std::setprecision(ss);
   os << "pad (error) = "<< std::setw(8) << bank.pad << std::endl;
   //if(bank.pad) 
   //  os << "PAD INFO: " << bank.intpts[MAX_INPUT-1];
   //for(int x=0;x<16;x++)
   for(int x=0;x<bank.num;x++)
      os << "\t" << bank.intpts[x];
   os << "************************" << std::endl;
   return os;
}



//static UShort_t SwapShort(UShort_t datum);
inline UShort_t SwapShort(UShort_t datum) {
  UShort_t temp = 0;
  temp = (datum&0x00ff);
  return (temp<<8) + (datum>>8);
}          

struct GEBMode3Head {
  UShort_t a2;
  UShort_t a1;
  UShort_t lengthGA;
  UShort_t board_id;
  //Int_t  GetLength()    const { return  lengthGA&0x07ff; }
  Int_t  GetLength()    const { return  lengthGA>>8; }
  Int_t  GetChannel()   const { return  board_id&0x000f; }
  Int_t  GetVME()       const { return (board_id&0x0030)>>4; }
  Int_t  GetCrystal()   const { return (board_id&0x00c0)>>6; }
  Int_t  GetHole()      const { return (board_id&0x1f00)>>8 ; }
  Int_t  GetSegmentId() const { return GetVME()*10+GetChannel(); }
  Int_t  GetCrystalId() const { return GetHole()*4+GetCrystal(); }
  friend std::ostream& operator<<(std::ostream& os, const GEBMode3Head &head);
}__attribute__((__packed__));


//static void SwapMode3Head(GEBMode3Head &head);
inline GEBMode3Head SwapMode3Head(const GEBMode3Head &in) {
  GEBMode3Head out = in;
  out.lengthGA = SwapShort(out.lengthGA);
  out.board_id = SwapShort(out.board_id);
  return out;
}         

inline std::ostream& operator<<(std::ostream& os, const GEBMode3Head &head) {
  os << "a1       = " << std::hex << head.a1 << std::endl;
  os << "a2       = " << std::hex << head.a2 << std::endl;
  os << "board_id = " << std::hex << head.board_id << std::endl;
  os << "lengthGA = " << std::hex << head.lengthGA << std::endl;
  return os;    
}         


//typedef struct {
struct GEBMode3Data {
  UShort_t led_middle;
  UShort_t led_low;
  UShort_t energy_low;
  UShort_t led_high;
  UShort_t cfd_low;
  UShort_t energy_high;
  UShort_t cfd_high;
  UShort_t cfd_middle;
  UShort_t cfd_pt1_high;
  UShort_t cfd_pt1_low;
  UShort_t cfd_pt2_high;
  UShort_t cfd_pt2_low;
  Long_t GetLed() const;
  Long_t GetCfd() const;
  UShort_t GetDeltaT1() const;
  UShort_t GetDeltaT2() const;

  Int_t  GetEnergy(const GEBMode3Head&) const;
  Int_t  GetEnergy0(const GEBMode3Head&) const;
  Int_t  GetEnergy1(const GEBMode3Head&) const;
  Int_t  GetEnergy2(const GEBMode3Head&) const;
  friend std::ostream& operator<<(std::ostream& os, const GEBMode3Data &data);
}__attribute__((__packed__));

//static void SwapMode3Data(GEBMode3Data &data);
inline GEBMode3Data SwapMode3Data(const GEBMode3Data &in) {
  //std::cout << __PRETTY_FUNCTION__ << std::endl;
  GEBMode3Data out = in;
  out.led_middle   = SwapShort(out.led_middle)   ;
  out.led_low      = SwapShort(out.led_low)      ;
  out.energy_low   = SwapShort(out.energy_low)   ;                                                                 
  out.led_high     = SwapShort(out.led_high)     ;
  out.cfd_low      = SwapShort(out.cfd_low)      ;
  out.energy_high  = SwapShort(out.energy_high)  ;
  out.cfd_high     = SwapShort(out.cfd_high)     ;
  out.cfd_middle   = SwapShort(out.cfd_middle)   ;
  out.cfd_pt1_high = SwapShort(out.cfd_pt1_high) ;
  out.cfd_pt1_low  = SwapShort(out.cfd_pt1_low)  ;
  out.cfd_pt2_high = SwapShort(out.cfd_pt2_high) ;
  out.cfd_pt2_low  = SwapShort(out.cfd_pt2_low ) ;
  return out;
}          

inline std::ostream& operator<<(std ::ostream& os, const GEBMode3Data &data) {
  os << "Led                 : " << data.GetLed() << "\n"
     << "Cfd                 : " << data.GetCfd() << "\n"
     //<< "Energy              : " << std::hex << "0x" << data.GetEnergy() << std::dec << "\n"
     << "Energy Low          : " << data.energy_low << "\n"
     << "Energy High         : " << data.energy_high << "\n"
     << "Energy Low          : " << std::hex << "0x" << data.energy_low << std::dec << "\n"
     << "Energy High         : " << std::hex << "0x" << data.energy_high << std::dec << "\n";
  return os;    
}          

inline Long_t   GEBMode3Data::GetLed() const { return (((long)led_high)<<32) + (((long)led_middle)<<16) + (((long)led_low)<<0); }
inline Long_t   GEBMode3Data::GetCfd() const { return (((long)cfd_high)<<32) + (((long)cfd_middle)<<16) + (((long)cfd_low)<<0); }
inline UShort_t GEBMode3Data::GetDeltaT1() const { return cfd_low; }
inline UShort_t GEBMode3Data::GetDeltaT2() const { return cfd_middle; }
            
inline Int_t  GEBMode3Data::GetEnergy(const GEBMode3Head &head) const  {
  int channel = head.GetChannel(); 
  //if ((channel==1) &&( head.GetHole()==9) && (head.GetCrystal()==3) && (head.GetVME()==3))
    //channel = 9; //  Q5 e5 has an inverted radial box, treat it as a core.  pcb.
  int  temp = (((int)energy_high)<<16) + (((int)energy_low)<<0);
  bool sign = temp&0x01000000;     
       temp = temp&0x00ffffff;     
  if(sign)  
    temp = temp - (int)0x01000000; 
  if(channel!=9)  // do not remove.  this is 100% needed. pcb.
    temp = -temp;                  
  return temp;                     
}           
            
inline Int_t  GEBMode3Data::GetEnergy0(const GEBMode3Head &head) const  {
  int channel = head.GetChannel(); 
  //if ((channel==1) &&( head.GetHole()==9) && (head.GetCrystal()==3) && (head.GetVME()==3))
    //channel = 9; //  Q5 e5 has an inverted radial box, treat it as a core.  pcb.
  int  eng_high_bit = (cfd_pt1_low & 0x01ff); 
  int  temp = (((int) eng_high_bit )<<16) + (((int)cfd_high)<<0);
   bool sign = temp&0x01000000;    
       temp = temp&0x00ffffff;     
  if(sign)  
    temp = temp - (int)0x01000000; 
  if(channel!=9)  // do not remove.  this is 100% needed. pcb.
    temp = -temp;                  
  return temp;                     
}           
            
inline Int_t  GEBMode3Data::GetEnergy1(const GEBMode3Head &head) const  {
  int channel = head.GetChannel(); 
  //if ((channel==1) &&( head.GetHole()==9) && (head.GetCrystal()==3) && (head.GetVME()==3))
    //channel = 9; //  Q5 e5 has an inverted radial box, treat it as a core.  pcb.
            
  int  eng_low_bit  = (cfd_pt1_low & 0xfe00) >>9; 
  int  eng_mid_bit  = (cfd_pt1_high);
  int  eng_high_bit = (cfd_pt2_low&0x0003);
            
  int  temp = (((int) eng_high_bit )<<23) + (((int)eng_mid_bit)<<7) + (((int)eng_low_bit)<<0) ;
            
   bool sign = temp&0x01000000;    
       temp = temp&0x00ffffff;     
  if(sign)  
    temp = temp - (int)0x01000000; 
  if(channel!=9)  // do not remove.  this is 100% needed. pcb.
    temp = -temp;                  
  return temp;                     
}           

inline Int_t  GEBMode3Data::GetEnergy2(const GEBMode3Head &head) const  {
  int channel = head.GetChannel();
  //if ((channel==1) &&( head.GetHole()==9) && (head.GetCrystal()==3) && (head.GetVME()==3))
    //channel = 9; //  Q5 e5 has an inverted radial box, treat it as a core.  pcb.
           
  int  eng_low_bit  = (cfd_pt2_low &0xfffc)>>2; 
  int  eng_high_bit = (cfd_pt2_high & 0x07ff); 
           
  int  temp = (((int) eng_high_bit )<<14) + (((int)eng_low_bit)<<0);
           
   bool sign = temp&0x01000000;
       temp = temp&0x00ffffff;
  if(sign) 
    temp = temp - (int)0x01000000;
  if(channel!=9)  // do not remove.  this is 100% needed. pcb.
    temp = -temp;
  return temp;
}          





























struct GEBS800Header {
  Int_t    total_size;
  UShort_t total_size2;
  UShort_t S800_packet;
  UShort_t S800_packet_size;
  UShort_t S800_version;
  UShort_t S800_timestamp_packet;
  ULong_t  S800_timestamp;
  UShort_t S800_eventnumber_packet_size;
  UShort_t S800_eventnumber_packet;
  UShort_t S800_eventnumber_low;
  UShort_t S800_eventnumber_middle;
  UShort_t S800_eventnumber_high;
  Long_t GetEventNumber() const {
     long temp = (long)S800_eventnumber_low;
     temp     += ((long)S800_eventnumber_middle) << 16;
     temp     += ((long)S800_eventnumber_high)   << 32;
     return temp;
  }

  friend std::ostream& operator<<(std::ostream& os, const GEBS800Header &head);
} __attribute__((__packed__));


static Int_t GetS800Channel(UShort_t input) { return (input>>12);    }
static Int_t GetS800Value(UShort_t input)   { return (input&0x0fff); }


struct S800TriggerPacket {
  UShort_t trgger_pattern;
  UShort_t channel_time[4];
  UShort_t channel_time_number;
  friend std::ostream& operator<<(std::ostream& os, const S800TriggerPacket &pack);
} __attribute__((__packed__));


struct S800TOFPacket {
  UShort_t value[4];
  UShort_t number;
friend std::ostream& operator<<(std::ostream& os, const S800TOFPacket &tof);
} __attribute__((__packed__));


struct S800SCINTPacket {
  UShort_t value[2][4];
  UShort_t number;
} __attribute__((__packed__));

// https://wikihost.nscl.msu.edu/S800Doc/doku.php?id=s800_version_0x0006

struct S800FPICPacket {
  UShort_t number;
  UShort_t subid;
  UShort_t value[16];
} __attribute__((__packed__));

struct S800FPCRDCPacket {
  UShort_t id;
  UShort_t number;
  UShort_t subid;
  /// not finished.
} __attribute__((__packed__));

typedef struct PWHit {
   Short_t pix_id;  //int16_t
   Short_t data_a;  //int16_t
   Short_t data_b;  //int16_t
   Short_t data_c;  //int16_t
   Short_t time;    //int16_t
friend std::ostream& operator<<(std::ostream& os,const PWHit &hit);
}__attribute__((__packed__)) PWHit;


typedef struct PWBank{
   PWHit hit[MAX_PWID];
   Short_t nhits;       //int16_t   -> not actually part of the bank recorded to file.
friend std::ostream& operator<<(std::ostream& os,const PWBank &bank);
}__attribute__((__packed__)) PWBank;


typedef struct LaBrSeg{
  Short_t chan_id;   //int16_t
  Short_t value;     //int16_t
friend std::ostream& operator<<(std::ostream& os,const LaBrSeg &hit);
}__attribute__((__packed__)) LaBrSeg;


typedef struct LaBrBank {
  Short_t nenghits;
  Short_t ntimhits;
  LaBrSeg energy_hit[MAX_LABRID];
  LaBrSeg time_hit[MAX_LABRID];
friend std::ostream& operator<<(std::ostream& os,const LaBrBank &bank);
}__attribute__((__packed__)) LaBrBank;


typedef struct RawHeader {
  Int_t datum1;
  Int_t datum2;
friend std::ostream& operator<<(std::ostream& os, const RawHeader &head);
} __attribute__((__packed__)) RawHeader;

//typedef struct GEBHeader : public RawHeader {
//  Int_t type()      { return datum1; } //Int_t  type; //int32_t
//  Int_t size()      { return datum2; } //int32_t
typedef struct GEBheader {
  int32_t type;
  int32_t size;
  int64_t timestamp;
} __attribute__((__packed__)) GEBHeader;

inline std::ostream& operator<<(std::ostream& os, const GEBHeader &head) { 
  os << "type: " << head.type << "\t"
     << "size: " << head.size << "\t" 
     << "@\t"    << head.timestamp;
  return os;
}

typedef struct EVTHeader : public RawHeader {
  Int_t size()      { return datum1; } //Int_t  size;       //int32_t
  Int_t type()      { return datum2; } //Int_t  type;       //int32_t
  ClassDef(EVTHeader, 0);
} __attribute__((__packed__)) EVTHeader;


typedef struct NSCLBodyHeader {
  int body_header_size;
  long timestamp;
  int sourceid;
  int barrier;
} __attribute__((__packed__)) NSCLBodyHeader;

typedef struct TNSCLFragmentHeader {
  long timestamp;
  int sourceid;
  int payload_size;
  int barrier;
friend std::ostream& operator<<(std::ostream& os, const TNSCLFragmentHeader &head);
} __attribute__((__packed__)) TNSCLFragmentHeader;


typedef struct TNSCLBeginRun {
  unsigned int run_number;
  unsigned int time_offset;
  unsigned int unix_time;
  unsigned int offset_divisor;
  char title[81];
friend std::ostream& operator<<(std::ostream& os, const TNSCLBeginRun& begin);
} __attribute__((__packed__)) TNSCLBeginRun;


#endif
