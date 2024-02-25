// normcc.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// February 26, 2024

#ifndef NORMCCHEADERDEF
#define NORMCCHEADERDEF

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Signal
{
private:
  int startIndex;
  int duration;
  int endIndex;
  double *data;

public:
  // empty constructor
  Signal();

  // import function
  bool SignalImport(string fileName);

  // export function
  void SignalExport(string fileName);

  // crosscorrelation function
  double computeXcorr(Signal x, Signal y, int lag);

  // normalized crosscorrelation function
  Signal normalizedXCorr(Signal x, Signal y);
};

#endif
