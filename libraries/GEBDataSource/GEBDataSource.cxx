
#include <GEBDataSource.h>

//#include <fstream>
//#include <iostream>
//#include <queue>
//#include <vector>
//#include <cstdint>

GEBDataSource::GEBDataSource() : RDataSource() { } 

GEBDataSource::~GEBDataSource() { } 

bool  GEBDataSource::HasColumn(std::string_view colName) const {
  return fColumnNames.end() != std::find(fColumnNames.begin(), fColumnNames.end(), colName);
}
