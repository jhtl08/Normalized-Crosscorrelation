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
  double * raw;
  double * data;
  double * normXcorrData;

public:
  // empty constructor
  Signal();

  // import function
  void SignalImport(std::string fileName);

  // export function
  void SignalExport(std::string fileName);

  // if the duration of the signal is less than 20
  void SignalcmdPrint();

  // converts the raw signal into data
  void SignalData();

  // crosscorrelation function
  double computeXcorr(Signal x, Signal y, int lag);

  // normalized crosscorrelation function
  Signal listXcorr(Signal x, Signal y);
};

#endif