#ifndef HISTOSVC_HH
#define HISTOSVC_HH

#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include <vector>
#include <string>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using namespace std;

class HistoSvc{
  
public:
  HistoSvc(){;};
  ~HistoSvc();
  virtual void HistoInit(string fname); 
  virtual void HistoExit();
  virtual void AddHisto(string,TH1 *);
  virtual TH1* GetHisto(string);
  
  static HistoSvc* GetInstance();

private:
  static HistoSvc* fInstance;

  map<string,TH1 *> fHistoMap;
  TFile* fHistoFile;


  
};


#endif
