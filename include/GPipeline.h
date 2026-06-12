#ifndef __GPIPELINE_H__
#define __GPIPELINE_H__

#include<string>

class GPipeline {
  public:
    GPipeline() = default;
    ~GPipeline() = default;

    void Sort(const std::string& fname);

};


#endif
