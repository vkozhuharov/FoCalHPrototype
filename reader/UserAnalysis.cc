#include "HistoSvc.hh"
#include "ProtoFocalEvent.hh"
#include <iostream>
#include <iomanip>
#include "TCanvas.h"

 
int InitAnalysis() {
  //Book the histograms
  HistoSvc *histo = HistoSvc::GetInstance();
  
  char name[256];
  char title[256];

  for(int ib = 0;ib < MAXNBOARDS; ib++) {
    for (int i = 0;i< MAXNCHANNELS;i++){
      sprintf(name,"B_%d_Ch_%02d",ib,i);
      sprintf(title,"Charge in channel %02d , board %d",i,ib);
      histo->AddHisto(name, new TH1F(name, title, 1000, 0.0, 4096));
    }
  }
  histo->AddHisto("ProtoFoCalMap",new TH2F("ProtoFoCalMap","Charge map of a single event",NROWS,0,NROWS,NCOLUMNS,0,NCOLUMNS));
  histo->AddHisto("ProtoFoCalMapComulative",new TH2F("ProtoFoCalMapComulative","Charge map of a single event",NROWS,0,NROWS,NCOLUMNS,0,NCOLUMNS));

  
  

  //Do whatever you want 
  
  return 0;
}



int AnalyzeEvent(ProtoFoCalHEvent *evt ){
  HistoSvc *histo = HistoSvc::GetInstance();

  char name[256];
  static int N;
  N++;
  
  //  printSingleEvent(evt);
  
  if(evt->bOK[1] == 1) return 0;
		       
  if(evt->t0event == 0 && evt->t1event == 0 ) {
    //Good event !

    for(int x = 0;x<NROWS;x++){
      for(int y=0;y<NCOLUMNS;y++){
	//	histo->GetHisto("ProtoFoCalMap")->SetBinContent(NROWS-x,NCOLUMNS-y,evt->ch[x][y].charge);
	((TH2F* ) histo->GetHisto("ProtoFoCalMapComulative")) -> Fill(NROWS-x-1,NCOLUMNS-y-1,evt->ch[x][y].charge);
	
      }
    }

    TCanvas *c = new TCanvas();
    
    // c->cd();
    // histo->GetHisto("ProtoFoCalMap")->Draw("colz");
    // histo->GetHisto("ProtoFoCalMap")->SetStats(0);
    // //    c->SetLogz();
    // sprintf(name,"pics/ev%d.png",N);
    // c->Update();
    // c->Print(name);
    // delete c;

  }

  
  

  
  

  return 0;
}


int printSingleEvent(ProtoFoCalHEvent *evt ){
  std::cout << "EvN: "<< std::setw(8) << evt->evn
	    << " t0_0: " << std::setw(10) << evt->t0[0]
	    << " t1_0: " <<  std::setw(10) << evt->t1[0]
	    << " t0_1: " << std::setw(10) << evt->t0[1]
	    << " t1_1: " <<  std::setw(10) << evt->t1[1]
    
	    // << " ts0_ref: " << evt->ch[0][0].ts0_ref
    	    // << " ts1_ref: " << evt->ch[0][0].ts1_ref
	    << std::endl;

  
  return 0;
}



int EndAnalysis(){
  //May be analyze the filled in histos and fill others
  HistoSvc *histo = HistoSvc::GetInstance();



  return 0;
}

