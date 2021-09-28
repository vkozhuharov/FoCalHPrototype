#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>      // std::ifstream
#include <signal.h>
#include <sstream>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TObjArray.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "HistoSvc.hh"
#include "ProtoFocalEvent.hh"

#define TIMEWINDOW 500 //500 ns time window for event matching
#define TS1LIMIT 10


//TO BE DONE

//matching between two files with different trees

void usage(char *name){
  std::cout << name << "  -l board_with_MAC_0   -r  board_with_MAC_1" << std::endl;
  return;
}

void sighandler(int sig){
    std::cerr << std::endl << "********************************************************************************" << std::endl;
    std::cerr << "Killed with Signal " << sig << std::endl << "Closing ROOT files ..." << std::endl;

    HistoSvc* histos = HistoSvc::GetInstance();
    histos->HistoExit();

    std::cerr << "... Histogram file Done" << std::endl;
    std::cerr << std::endl << "********************************************************************************" << std::endl;

    exit(0);
}

class ProtoFoCalHConfig {
public:
  void ReadConfig();
  void PrintConfig();
  int GetX(int mac,int ich){return X[mac][ich];};
  int GetY(int mac,int ich){return Y[mac][ich];};
  //For the moment MAC addresses go consequitively
  
public:
  int X[2][32];
  int Y[2][32];
  int ncards;  
};

void ProtoFoCalHConfig::PrintConfig(){
  std::cout << "====== FoCal test configuration =====" << std::endl;
  for(int imac = 0;imac<ncards;imac++) {
    for(int ich = 0;ich < 32;ich++){
      std::cout << imac << "   " << ich << "    " << X[imac][ich] << "   " << Y[imac][ich] << std::endl;
    }
  }

}


void ProtoFoCalHConfig::ReadConfig(){
  std::ifstream filein ("FoCal.conf", std::ifstream::in);
  std::string line;

  //Get the first non-comment line
  //Should contain the number of cards
  for (std::string line; std::getline(filein, line); ){
    std::cout << line << std::endl;
    if(line.find("#") == 0) {
      continue;
    } else {
      std::stringstream ss(line);
      ss >> ncards;
      break;
    }
  }

  //Book space for the channels mapping
  //Leave it for the moment --TO BE DONE--
  //

  int mac; int ch; int x; int y;
  for (std::string line; std::getline(filein, line); ){
    std::cout << line << std::endl;
    if(line.find("#") == 0) continue;
    std::stringstream ss(line);
    ss >> mac >> ch >> x >> y;
    
    if(mac >= 0 && ch >= 0 && ch < 32) {
      X[mac][ch] = x;
      Y[mac][ch] = y;
    } else {
      
    }
  }
}







const int nChannels = 32;
const int nboards = 2;





TH1F *Charge[2][nChannels];
TH2F *ProtoFoCalMap;
TH2F *ProtoFoCalMapComulative;



void initHistos() {
  char name[256];
  char title[256];

  for(int ib = 0;ib < nboards; ib++) {
    for (int i = 0;i<nChannels;i++){
      sprintf(name,"B_%d_Ch_%02d",ib,i);
      sprintf(title,"Charge in channel %02d , board %d",i,ib);
      Charge[ib][i] = new TH1F(name, title, 1000, 0.0, 4096);
    }
  }
  ProtoFoCalMap = new TH2F("ProtoFoCalMap","Charge map of a single event",NROWS,0,NROWS,NCOLUMNS,0,NCOLUMNS);
  ProtoFoCalMapComulative = new TH2F("ProtoFoCalMapComulative","Charge map of a single event",NROWS,0,NROWS,NCOLUMNS,0,NCOLUMNS);
 
  
}

void resetHistos(){
  
  for(int ir=0;ir<NROWS;ir++){
    for(int ic=0;ic<NCOLUMNS;ic++) {
      ProtoFoCalMap->SetBinContent(ir+1,ic+1,0);
    }
  }
}


int readSingleEvent(ProtoFoCalHEvent *evt ){
  memset(&evt,0,sizeof(ProtoFoCalHEvent));
  
  return 0;
}

int clearEvent(ProtoFoCalHEvent *evt ){
  for(int x = 0;x<NROWS;x++){
    for(int y = 0; y<NCOLUMNS;y++) {
      evt->ch[x][y].charge  = 0;
      evt->ch[x][y].bid     = 0;
      evt->ch[x][y].ts0     = 0;
      evt->ch[x][y].ts1     = 0;
      evt->ch[x][y].ts0_ref = 0;
      evt->ch[x][y].ts1_ref = 0;      
    }
  }
  
  evt->t0event = 0;
  evt->t1event = 0;
  evt->charge = 0.;
  evt->time = 0.;
  
  
  return 0;
}






int main(int argc, char **argv){

  signal(SIGXCPU,sighandler);
  signal(SIGINT,sighandler);
  signal(SIGTERM,sighandler);
  signal(127,sighandler);
  
  ProtoFoCalHEvent event;

   char b0FileName[256];
   char b1FileName[256];

  int ne = 0;
  int nopt;
  int opt;

  int nl=0;
  int nr=0;
  
  extern char *optarg;

  while ((opt = getopt(argc, argv, "l:r:")) != -1) {
    switch (opt) {
      case 'l':
	strncpy(b0FileName,optarg,250);
	nl++;
        break;
	
      case 'r':
	strncpy(b1FileName,optarg,250);
	nr++;
        break;		
    default:
      break;
      usage(argv[0]);
      return 0;
    }
  }

  if( nl!=1 || nr!=1) {
    usage(argv[0]);
    return 0;
  }
  std::cout << "Using " << b0FileName << " with MAC5=0  and "<<b1FileName << " with MAC5=1 " << std::endl;

  //return 0;
  
  //int FoCalRead(){
  TFile *f = new TFile(b0FileName);
  TTree *t1 = (TTree*)f->Get("mppc");
  TFile *f2 = new TFile(b1FileName);
  TTree *t2 = (TTree*)f2->Get("mppc2");

  





  
  HistoSvc* histo = HistoSvc::GetInstance();
  histo->HistoInit("Output.root");
  
  InitAnalysis();
  
  char name[256];
  
  UShort_t chg[32];
  UInt_t ts0,ts1;
  UInt_t ts0_ref, ts1_ref;
  UChar_t mac5;


  ProtoFoCalHConfig cfg;

  struct ProtoFoCalHBoard *evt[MAXNCARDS];

  
  cfg.ReadConfig();
  cfg.PrintConfig();
  
  if(cfg.ncards > MAXNCARDS) return 0;

  for(int ic = 0;ic < cfg.ncards; ic ++) {
    evt[ic] = new ProtoFoCalHBoard;
  }

  
  //  return 0;
  
  // initHistos();
  
  TObjArray* branches = t1->GetListOfBranches();
  Int_t nentries = (Int_t)t1->GetEntries();
  Int_t nentries2 = (Int_t)t2->GetEntries();

  std::cout << "Found tree with " <<  nentries << " entries" << std::endl;
  std::cout << "Found tree with " <<  nentries2 << " entries" << std::endl;


  
  t1->SetBranchAddress("chg",evt[0]->chg);
  t1->SetBranchAddress("ts0",&(evt[0]->ts0));
  t1->SetBranchAddress("ts1",&(evt[0]->ts1));
  t1->SetBranchAddress("ts0_ref",&(evt[0]->ts0_ref));
  t1->SetBranchAddress("ts1_ref",&(evt[0]->ts1_ref));
  t1->SetBranchAddress("mac5",&(evt[0]->mac5));

  t2->SetBranchAddress("chg",evt[1]->chg);
  t2->SetBranchAddress("ts0",&(evt[1]->ts0));
  t2->SetBranchAddress("ts1",&(evt[1]->ts1));
  t2->SetBranchAddress("ts0_ref",&(evt[1]->ts0_ref));
  t2->SetBranchAddress("ts1_ref",&(evt[1]->ts1_ref));
  t2->SetBranchAddress("mac5",&(evt[1]->mac5));

  int iEv0 = 0;
  int iEv1 = 0;
  int nFrames = 0;

  int nFrames1 = 0;
  int nFrames2 = 0;

  int nTS1events = 0;
  int nTS1events1 = 0;
  int nTS1events2 = 0;

  int TS1timePrev1 = 0.;
  int TS1timePrev2 = 0.;

  int iEv = 0; //Global event number sent for analysis

  
  //position at the first TS0 signal
  do{
    t1->GetEntry(iEv0);
    iEv0++;
  } while (evt[0]->ts0 != 0 || iEv0 == nentries);

  if(iEv0 < nentries) {
    std::cout << "B0: First event number with reference signal: " << iEv0-1 << "   TimeStamp TS0: " << evt[0]->ts0 << std::endl;
  } else {
    std::cout << "No reference signal found in the data" << std::endl;
    
  }
  
  //position at the first TS0 signal

  do{
    t2->GetEntry(iEv1);
    iEv1++;
  } while (evt[1]->ts0 != 0 || iEv1 == nentries2);
  
  if(iEv0 < nentries) {  
    std::cout << "B1: First event number with reference signal: " << iEv1-1 << "   TimeStamp TS0: " << evt[1]->ts0 << std::endl;
  } else {
    std::cout << "No reference signal found in the data" << std::endl;
  }
  

  if(iEv0 < nentries && iEv1 < nentries2) {
    std::cout << "Found common mark event, proceeding" << std::endl;
  } else {
    return 0; //Or not.... ?
  }
 
  //Form the event - first event, special event, ........
  iEv++;
  event.evn = iEv;
  event.t0event = 1;
  event.t0event = 0;
  for(int ic = 0;ic < cfg.ncards; ic++) {
    event.t0[ic] = evt[ic]->ts0;
    event.t0ref[ic] = evt[ic]->ts0_ref;
    event.t1[ic] = evt[ic]->ts1;
    event.t1ref[ic] = evt[ic]->ts1_ref;
    for (int iCh = 0;iCh<nChannels;iCh++){
      if(cfg.GetX(ic,iCh) >= 0 && cfg.GetY(ic,iCh) >= 0 ){
	//Fill the event structure;
	//Get X, Get Y
	int x  = cfg.GetX(ic,iCh);
	int y = cfg.GetY(ic,iCh);
	
	event.ch[x][y].charge = evt[ic]->chg[iCh];
	event.ch[x][y].bid = evt[ic]->mac5;
	event.ch[x][y].ts0 = evt[ic]->ts0;
	event.ch[x][y].ts1 = evt[ic]->ts1;
	event.ch[x][y].ts0_ref = evt[ic]->ts0_ref;
	event.ch[x][y].ts1_ref = evt[ic]->ts1_ref;
      }      
    }
  }
  
  //Analyze the event
  std::cout << "Analize the first special event " << std::endl;
  AnalyzeEvent(&event);


  //Start the event loop:
  do {
    t1->GetEntry(iEv0);
    iEv0++;
    t2->GetEntry(iEv1);
    iEv1++;

    
    while( fabs(1.*evt[0]->ts0 - 1.*evt[1]->ts0) > TIMEWINDOW  && iEv1 < nentries2 && iEv0<nentries) {
      //Check which one to advance
      if( evt[0]->ts0 > evt[1]->ts0 + TIMEWINDOW ||
	  evt[0]->ts0 == 0 //reached end of frame
	  ) {
	t2->GetEntry(iEv1);
	iEv1++;
      } else 
      // if( evt[1]->ts0 >  evt[0]->ts0 + TIMEWINDOW ||
      // 	  evt[1]->ts0 == 0 //reached end of frame
      // 	  )
	{
	t1->GetEntry(iEv0);
	iEv0++;
      }      
    }

    
    //    std::cout << "Matching found at " << iEv0 << "   " << iEv1 << std::endl; 

    if (iEv1 < nentries2 && iEv0<nentries) {
      //Found matching timestamps.
      //Fill and analize the event
      
      clearEvent(&event);
      if(evt[0]->ts0 == 0 &&   evt[1]->ts0 == 0) {
	event.t0event = 1;
	nFrames++;
	std::cout << "Found new T0 signal. Number of T1 triggers in previous frame:" << nTS1events << std::endl;
	if(nTS1events > 10) {
	  std::cout << "==== WARNING: Missed T0 clock identified ====" << std::endl;
	}
	nTS1events=0;
      } else {
	event.t0event = 0;
      }
      if(evt[0]->ts1 == 0 &&   evt[1]->ts1 == 0) {
	event.t1event = 1;
	nTS1events++;
      } else {
	event.t1event = 0;
      }


      for(int ic = 0;ic < cfg.ncards; ic++) {
	event.t0[ic] = evt[ic]->ts0;
	event.t0ref[ic] = evt[ic]->ts0_ref;
	event.t1[ic] = evt[ic]->ts1;
	event.t1ref[ic] = evt[ic]->ts1_ref;
	for (int iCh = 0;iCh<nChannels;iCh++){
	  if(cfg.GetX(ic,iCh) >= 0 && cfg.GetY(ic,iCh) >= 0 ){
	    //Fill the event structure;
	    //Get X, Get Y
	    int x  = cfg.GetX(ic,iCh);
	    int y = cfg.GetY(ic,iCh);
	    
	    event.ch[x][y].charge = evt[ic]->chg[iCh];
	    event.ch[x][y].bid = evt[ic]->mac5;
	    event.ch[x][y].ts0 = evt[ic]->ts0;
	    event.ch[x][y].ts1 = evt[ic]->ts1;
	    event.ch[x][y].ts0_ref = evt[ic]->ts0_ref;
	    event.ch[x][y].ts1_ref = evt[ic]->ts1_ref;
	  }      
	}
      }
      //Analyze the event
      iEv++;
      event.evn = iEv;      
      AnalyzeEvent(&event);   
    }
    
   
    
  } while (iEv1 < nentries2 && iEv0 < nentries);



  





  /*
  return 0;
  

  
  for (Int_t iEv=0; iEv<nentries; iEv++) {

    
    clearEvent(&event);
    //    resetHistos();
    t1->GetEntry(iEv);
    //    if (cfg.ncards > 1) 
    //t2->GetEntry(iEv);

    event.evn = iEv;

    for(int ic = 0;ic < cfg.ncards; ic++) {
      for (int iCh = 0;iCh<nChannels;iCh++){
	sprintf(name,"B_%d_Ch_%02d",ic,iCh);

	if(evt[ic]->ts0 == 0)
	  histo->GetHisto(name)->Fill(evt[ic]->chg[iCh]);
	
	if(cfg.GetX(ic,iCh) >= 0 && cfg.GetY(ic,iCh) >= 0 ){
	  //Fill the event structure;
	  //Get X, Get Y
	  int x  = cfg.GetX(ic,iCh);
	  int y = cfg.GetY(ic,iCh);

	  event.ch[x][y].charge = evt[ic]->chg[iCh];
	  event.ch[x][y].bid = evt[ic]->mac5;
	  event.ch[x][y].ts0 = evt[ic]->ts0;
	  event.ch[x][y].ts1 = evt[ic]->ts1;
	  event.ch[x][y].ts0_ref = evt[ic]->ts0_ref;
	  event.ch[x][y].ts1_ref = evt[ic]->ts1_ref;
	  
	  if(evt[ic]->chg[iCh] > 300)
	    histo->GetHisto("ProtoFoCalMap")->SetBinContent( NROWS - cfg.GetX(ic,iCh),NCOLUMNS-cfg.GetY(ic,iCh),evt[ic]->chg[iCh]);
	}
      }
    }


    
    if(iEv %10000 == 0) {
      std::cout << "Processed " << iEv << " events" << std::endl;
    }

    // c->cd();
    // ProtoFoCalMap->Draw("colz");
    // ProtoFoCalMap->SetStats(0);
    // //    c->SetLogz();
    // sprintf(name,"pics/ev%d.png",iEv);
    // c->Update();
    // c->Print(name);
    
    //sleep(1);
    

    AnalyzeEvent(&event);
    
  }

  */

  
  EndAnalysis();
    
  std::cout << "Read " << nentries << " events " << std::endl;
  
  //End of event loop
  // c->cd();
  // ProtoFoCalMap->Draw();
 
  // TCanvas *c1 = new TCanvas();

  // c1->Divide(8,4);
  // for (int iCh = 0;iCh<nChannels;iCh++){
  //   c1->cd(iCh+1);
  //   Charge[0][iCh]->Draw();;      
  // }
  // TCanvas *c2 = new TCanvas();

  // c2->Divide(8,4);
  // for (int iCh = 0;iCh<nChannels;iCh++){
  //   c2->cd(iCh+1);
  //   Charge[1][iCh]->Draw();;      
  // }


  

  
  // getchar();


  std::cout << "Trying to enter HistoExit  " << std::endl;

  histo->HistoExit();

  return 0;
}
