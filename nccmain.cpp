//normccmain.cpp
//Kyle Coloma, Jason Lorenzo
//ENGG 151.01-A
//February 5, 2022

#include "normcc.h"
using namespace std;

int main(int argc, char* argv[])
{
  Signal x;
  Signal y;
  x.SignalImport("x.txt");
  x.SignalExport("testx");
  cout<<endl;
  y.SignalImport("y.txt");
  y.SignalExport("testy");
}