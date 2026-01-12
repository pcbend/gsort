
#include<cstdio>
#include<signal.h>


#include<Gint.h>
#include<utils.h>
#include<globals.h>

#include<TROOT.h>
#include<TEnv.h>
//#include<TStyle.h>
//#include<TROOT.h>

void SigHandler(int) { 
  gShutdown.store(true);
}

void loadEnv() {
  // Set the GSORTSYS variable based on the executable path.
  // If GSORTSYS has already been defined, don't overwrite.
  setenv("GSORTSYS", (programPath()+"/..").c_str(), 0);

  printf("GSORTSYS has been set to: %s\n",getenv("GSORTSYS"));

  // Load $GSORTSYS/.gsortrc
  std::string gsortrc_path = Form("%s/.gsortrc",getenv("GSORTSYS"));
  gEnv->ReadFile(gsortrc_path.c_str(),kEnvChange);

  // Load $HOME/.gsortrc
  gsortrc_path = Form("%s/.gsortrc",getenv("HOME"));
  if(fileExists(gsortrc_path.c_str())){
    gEnv->ReadFile(gsortrc_path.c_str(),kEnvChange);
  }
  
  gsortrc_path = Form("%s/.gsortrc",getenv("PWD"));
  if(fileExists(gsortrc_path.c_str())){
    gEnv->ReadFile(gsortrc_path.c_str(),kEnvChange);
  }
}


int main(int argc, char **argv) {

  ROOT::EnableThreadSafety();

  //loadStyle();
  loadEnv();
  gROOT->ProcessLine(".L include/Gtypes.h"); // for some reason the enums aren't being processed properly by the rootclang

  //signal(SIGINT,SigHandler);
  signal(SIGTERM,SigHandler);
  
  Gint::Get(argc,argv)->Run(true);
 
  return 0;
}
