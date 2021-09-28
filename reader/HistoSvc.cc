#include "HistoSvc.hh"
#include <iostream>

class HistoSvc;


HistoSvc* HistoSvc::fInstance = 0;

HistoSvc* HistoSvc::GetInstance(){
  if(fInstance) return fInstance;
  fInstance = new HistoSvc();
  return fInstance;
}


void HistoSvc::HistoInit(string fname){
  fHistoFile = new TFile(fname.c_str(),"RECREATE");
  
  
  ;
}

void HistoSvc::AddHisto(string name, TH1 *histo){
  fHistoMap[name] = histo;
}

TH1*  HistoSvc::GetHisto(string name){
  return fHistoMap[name];
}


void HistoSvc::HistoExit(){
 
  std::cout << "Entering HistoExit  " << std::endl;
  
  if(fHistoFile == 0) 
    return;

  fHistoFile->cd();
  
  map<string, TH1*>::iterator itr;
  for(itr = fHistoMap.begin(); itr != fHistoMap.end(); itr++)
    {
      TH1* histo = itr->second;
      histo->Write();
      delete histo;
    }

  fHistoFile->Close();
}


HistoSvc::~HistoSvc()
{
  if ( fHistoFile ) delete fHistoFile;
}
