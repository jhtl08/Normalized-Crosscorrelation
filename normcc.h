// normcc.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// February 5, 2022

#ifndef NORMCCHEADERDEF
#define NORMCCHEADERDEF

#include <fstream>
#include <iostream>
#include <vector>

class Signal
{
private:
  int startIndex;
  int duration;
  int endIndex;
  double *elements;

public:
  // empty constructor
  Signal();

  // non-empty constructor
  Signal(double *signalArray, int inputStart, int inputDuration);

  // import function
  void SignalImport(std::string fileName);

  // export function
  void SignalExport(std::string fileName);

  // function to print in command line when duration<20
  void SignalcmdPrint();

  // crosscorrelation function
  double computeXcorr(Signal x, Signal y, int lag);

  // normalized crosscorrelation function
  Signal listXcorr(Signal x, Signal y);
};

#endif