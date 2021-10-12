#include <signal.h>
#include <iostream>
#include "HistoSvc.hh"
#include <sys/stat.h>
#include <unistd.h>


int FoCalHReader(char* ,char *);


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


int main(int argc, char **argv){

  signal(SIGXCPU,sighandler);
  signal(SIGINT,sighandler);
  signal(SIGTERM,sighandler);
  signal(127,sighandler);
  

   char b0FileName[256];
   char b1FileName[256];

  int ne = 0;
  int nopt;
  int opt;

  int nl=0;
  int nr=0;
  int nn=0;
  int nEventsToRead;
  
  extern char *optarg;

  while ((opt = getopt(argc, argv, "l:r:n:")) != -1) {
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
  
  FoCalHReader(b0FileName, b1FileName);
  
  return 0;

}
