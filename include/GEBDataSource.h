#ifndef __GEBDATASOURCE_H__
#define __GEBDATASOURCE_H__

#include<vector>
#include<string>

#include <ROOT/RDataSource.hxx>

class GEBDataSource : public ROOT::RDF::RDataSource {

  public:
    GEBDataSource();
    ~GEBDataSource();

    const std::vector<std::string>& GetColumnNames() const { return fColumnNames; }
    bool  HasColumn(std::string_view colName) const; 

  private:
    std::vector<std::string> fColumnNames; 

  ClassDef(GEBDataSource,0)
};

#endif

