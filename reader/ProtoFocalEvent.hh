#ifndef PROTOFOCALEVENT_HH
#define PROTOFOCALEVENT_HH

#define MAXNCARDS 32
#define MAXNBOARDS 2
#define MAXNCHANNELS 32

#define NROWS 8
#define NCOLUMNS 6

struct ProtoFoCalHBoard {
  UShort_t chg[32];
  UInt_t ts0,ts1;
  UInt_t ts0_ref, ts1_ref;
  UChar_t mac5;
};

struct ProtoFoCalHChannel{
  int charge;
  int bid;
  long int ts0,ts1;
  long int ts0_ref, ts1_ref;
};

struct ProtoFoCalHEvent{
  struct ProtoFoCalHChannel ch[NROWS][NCOLUMNS];
  long int t0[2];
  long int t0ref[2];
  long int t1[2];
  long int t1ref[2];
  float energy;
  float time;
  float charge;
  int evn;
  int t0event;
  int t1event;
  int bOK[2];
};

int InitAnalysis();
int AnalyzeEvent(ProtoFoCalHEvent *evt );
int printSingleEvent(ProtoFoCalHEvent *evt );
int EndAnalysis();


#endif
