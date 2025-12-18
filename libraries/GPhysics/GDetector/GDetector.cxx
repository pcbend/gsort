#include <GDetector.h>

GDetector::GDetector() {
  //Class()->CanIgnoreTObjectStreamer();
  Clear();
}

GDetector::GDetector(const char* name,const char* title) : TNamed(name,title) {
  Clear();
  //Class()->CanIgnoreTObjectStreamer();
}

GDetector::~GDetector() { }

void GDetector::Clear(Option_t *opt) {
  //TNamed::Clear(opt);
  fTimestamp = -1;
}

void GDetector::Print(Option_t *opt) const { }

void GDetector::Copy(TObject& obj) const {
  TNamed::Copy(obj);

  GDetector& det = (GDetector&)obj;
  det.fTimestamp = fTimestamp;
}

int GDetector::Compare(const TObject& obj) const {
  GDetector& det = (GDetector&)obj;
  if(fTimestamp < det.fTimestamp) {
    return -1;
  } else if (fTimestamp > det.fTimestamp) {
    return +1;
  } else {
    return 0;
  }
}

//int GDetector::Build(std::vector<TRawEvent>& raw_data){
//  int output = 0;
//  return output;
//}



