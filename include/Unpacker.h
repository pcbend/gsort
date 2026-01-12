#ifndef __UNPACKER_H__
#define __UNPACKER_H__

#include<Gtypes.h>

class GDetector;


//void unpack(const std::vector<rec> &event, const fileinfo &info,std::vector<gdetector>& out);
//void unpackgebtype1(const std::vector<rec> &event, const fileinfo &info,std::vector<gdetector>& out);
//void unpackevt(const std::vector<rec> &event, const fileinfo &info,std::vector<gdetector>& out);
//void unpackmid(const std::vector<rec> &event, const fileinfo &info,std::vector<gdetector>& out);


std::vector<std::unique_ptr<GDetector> > Unpack(const std::vector<Rec> &event, const FileInfo &info);
std::vector<std::unique_ptr<GDetector> > UnpackGEBTYPE1(const std::vector<Rec> &event, const FileInfo &info);
std::vector<std::unique_ptr<GDetector> > UnpackEVT(const std::vector<Rec> &event, const FileInfo &info);
std::vector<std::unique_ptr<GDetector> > UnpackMID(const std::vector<Rec> &event, const FileInfo &info);



#endif

