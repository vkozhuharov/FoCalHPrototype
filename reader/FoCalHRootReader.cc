#include "UserAnalysis.cc"
#include "HistoSvc.cc"
#include "ProtoFoCalHConfig.cc"
#include "FoCalHReader.cc"


int FoCalHRootReader(char* b0FileName,char *b1FileName){
  return FoCalHReader(b0FileName,b1FileName);
}
