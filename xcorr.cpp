// xcorr.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// February 26, 2024

#include "normcc.h"

using namespace std;

int main(int argc, char *argv[])
{
  // set file names from cmd terminal
  string inputFileName1 = argv[1];
  string inputFileName2 = argv[2];
  string outputFileName = argv[3];

  // Signal x and y
  Signal x;
  Signal y;

  cout << endl;

  // Signal x Import Signal Data
  bool checkXimport = x.SignalImport(inputFileName1);
  bool checkYimport = y.SignalImport(inputFileName2);

  // Check if valid
  if (!checkXimport || !checkYimport)
  {
    return 0;
  }

  // Compute for list of P_xy to be contained in Signal object
  Signal result = result.normalizedXCorr(x, y);

  // Export the normalized crosscorrelation values
  result.SignalExport(outputFileName);

  cout << endl;

  return 0;
}
