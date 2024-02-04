//normccmain.cpp
//Kyle Coloma, Jason Lorenzo
//ENGG 151.01-A
//February 5, 2022

#include "normcc.h"
using namespace std;

int main(int argc, char* argv[])
{
//set file names from cmd terminal
  string inputFileName1;
  string inputFileName2;
  string outputFileName;

  inputFileName1=argv[1];
  inputFileName2=argv[2];
  outputFileName=argv[3];

//Signal Import
  Signal x;
  Signal y;

  x.SignalImport(inputFileName1); 
  cout<<endl;
  y.SignalImport(inputFileName2);
  cout<<endl;
  
//Signal Export
  x.SignalExport(outputFileName+"x");
  cout<<endl;
  y.SignalExport(outputFileName+"y");
}