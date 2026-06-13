// GPipeline.cxx
#include <GPipeline.h>

#include <GFile.h>
#include <GEventBuilder.h>
#include <GPhysicsThread.h>
#include <GTTreeThread.h>
#include <GStatusThread.h>
#include <GHistogramer.h>
#include <GDetector.h>

void GPipeline::Sort(const std::string& fname) {
  GFile infile(fname);
  infile.start();

  std::string histName;
  if(infile.Info().subrun > 0)
    histName = Form("hist%04i_%03i.root", infile.Info().run, infile.Info().subrun);
  else
    histName = Form("hist%04i.root", infile.Info().run);

  GHistogramer::Get().SetOutFile(histName);

  GEventBuilder<Rec> eventbuilder(infile);
  eventbuilder.start();

  //GPhysicsThread<Rec> physics(eventbuilder, infile.Info());
  GPhysicsThread physics(eventbuilder, infile.Info());
  physics.start();

  using DetVec = std::vector<std::unique_ptr<GDetector>>;
  GTTreeThread<DetVec> treeThread(physics, "output.root");
  treeThread.start();

  GStatusThread status;
  status.start();

  status.join();
  GHistogramer::Get().Close();
}


