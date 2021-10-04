#ifndef PROTOFOCALCONFIG_H
#define PROTOFOCALCONFIG_H



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



#endif
