#ifndef __GTTreeThread_H__
#define __GTTreeThread_H__

#include <GThread.h>

#include <TFile.h>
#include <TTree.h>

#include <GBank88.h>

template<class In>
class GTTreeThread : public GThread<int> {
  public:
    using Event = In;//std::vector<std::unique_ptr<GDetector> >;

    explicit GTTreeThread(GThread<Event>& producer,
        std::string outname, std::string treename="eventTree") 
      : fProducer(producer),fOutName(std::move(outname)),fTreeName(std::move(treename)) { }

    ~GTTreeThread() override { Close(); } 

    void print() override {
      const char* state = "STOPPED";
      if (this->IsRunning())
        state = this->IsPaused() ? "PAUSED" : "RUNNING";

      std::cout << "[GTTree] State: " << state
        << " | Events: " << fEvents
        << " | InQ: " << fProducer.size()
        << " | Branches: " << fBranches.size()
        << " | File: " << (fFile ? fFile->GetName() : "none")
        << "\n";
    } 


    void Close() { 
      if(!fFile) return;
      TDirectory::TContext ctxt;
      fFile->cd();
      if(fTree) fTree->Write();
      fFile->Write();
      fFile->Close();
      fFile.reset();
    }

  private: 
    GThread<Event>& fProducer;
    std::string fOutName,fTreeName;

    std::unique_ptr<TFile> fFile;
    TTree *fTree = nullptr; 

    uint64_t fEvents{0};

    struct BranchInfo { 
      std::unique_ptr<GDetector> obj;
      void *addr = nullptr;
      TBranch* br = nullptr;
    };
    std::unordered_map<EDetector,BranchInfo> fBranches;

    bool Iteration() override {
      if(!fFile) {
        TDirectory::TContext ctxt;
        //fFile = std::make_unique<TFile>(TFile::Open(fOutName.c_str(),"recreate"));
        fFile.reset(TFile::Open(fOutName.c_str(),"recreate"));
        fTree = new TTree(fTreeName.c_str(),fTreeName.c_str());
  
        fTree->SetAutoFlush(1'000'000);
        fTree->SetAutoSave(0);
        fBranches.reserve(16);
      }
      Event event;
      if (!fProducer.pop(event)) {
        if (!fProducer.IsRunning() && fProducer.empty()) return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return true;
      }
      for(auto& [det,branchInfo] : fBranches) {
        branchInfo.obj->Clear();
      }
      for(auto& gdetector : event) { 
        const auto det = gdetector->GetDetector();

        auto it = fBranches.find(det);
        if(it==fBranches.end()) {
          //BranchInfo branchInfo;
          auto [insIt, inserted] = fBranches.emplace(det, BranchInfo{});
          it = insIt;
          auto& branchInfo = it->second;

          switch(det) {
            case EDetector::kGretina:
              continue;
            case EDetector::kMode3:
              continue;
            case EDetector::kDDAS:
              continue;
            case EDetector::kTigress:
              continue;
            case EDetector::kEMMA:
              continue;
            case EDetector::kBank88:
              branchInfo.obj = std::make_unique<GBank88>();
              branchInfo.addr = branchInfo.obj.get();
              branchInfo.br  = fTree->Branch("bank88","GBank88",&branchInfo.addr,32000,9);
              //branchInfo.br->SetBasketSize(256 * 1024);  // 256 KB (or 512 KB)
              //branchInfo.br->SetCompressionLevel(1);
              break;
            default:
              continue;
          };
          //fBranches.emplace(det,std::move(branchInfo));
        }
        gdetector->CopyTo(it->second.obj.get());
      }
      fTree->Fill();
      fEvents++;
      return true;
    }



};

#endif
