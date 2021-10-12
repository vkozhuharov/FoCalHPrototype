#  FoCalHPrototype

FoCal-H testbeam software and configurations


To run the code after compiling it on LINUX:  execute the script "build" by ./build

To run the code within the ROOT interpreter (Cling):

- run with
> root -l 'FoCalHRootReader.cc("data/2021_10_05_01_16_28/mppc0.root","data/2021_10_05_01_16_28/mppc1.root")'
where the arguments are the files from the board with MAC5 == 00 and the board with MAC5 == 01 respectively

- or start root by 
> root

and then 

root [0] .L FoCalHRootReader.cc
root [1] FoCalHRootReader("data/2021_10_05_01_16_28/mppc0.root","data/2021_10_05_01_16_28/mppc1.root");

