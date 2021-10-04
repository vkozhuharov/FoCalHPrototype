#include "ProtoFoCalHConfig.hh"
#include <iostream>
#include <fstream>      // std::ifstream
#include <signal.h>
#include <sstream>


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



