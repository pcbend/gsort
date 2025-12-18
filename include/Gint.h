#ifndef __GINT_H__
#define __GINT_H__

#ifndef __CINT__
#include <thread>
#endif

#include <TRint.h>

#include <Gtypes.h>

class TFile;

#ifndef __CINT__
class GintOptions {
  private:
    GintOptions() = default;
  public:
    ~GintOptions() = default;

    static void AddGEBFile(std::string fname)     { fGEBFiles.push_back(fname); }
    static std::vector<std::string> GetGEBFiles() { return fGEBFiles; }


  private:
    static std::vector<std::string> fGEBFiles;
};
#endif



class Gint : public TRint {
  private:
    Gint(int argc, char **argv);
    static Gint *fGint; 
  public:
    static Gint *Get(int argc=0,char **argv=0);
    virtual ~Gint();
  
    int  TabCompletionHook(char* buf, int* pLoc, std::ostream& out) override;
    long ProcessLine(const char *line, bool sync=true,int *error=0) override;

    void Terminate(int status) override;

    void Sort(std::string fname);


  public:
    void      LoadOptions(int argc, char **argv);
    void      ApplyOptions(); 
#if !defined(__ROOTCLING__) && !defined(__CINT__)
    EFileType DetermineFileType(const std::string& filename) const;
#endif
    //bool      FileAutoDetect(const std::string& filename);
    TFile*    OpenRootFile(const std::string& filename, Option_t *opt="");

    void      LoadStyle();    

  private:
    int fRootFilesOpened;
    bool fTabLock;

#ifndef __CINT__
    std::thread::id fMainThreadId;
#endif

  ClassDefOverride(Gint,0)
};



#endif

