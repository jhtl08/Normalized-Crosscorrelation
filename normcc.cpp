// normcc.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// February 5, 2022

#include "normcc.h"
#include <sstream>
#include <cmath>

using namespace std;

Signal::Signal()
{
  startIndex = 0;
  duration = 1;
  endIndex = 0;
  raw = new double[1];
  raw[0] = 0;
  data = new double[1];
  data[0] = 0;
  normXcorrData = new double[1];
  normXcorrData[0] = 0;

}

void Signal::SignalImport(string fileName)
{

  // opens and checks the files
  ifstream isignalFile;
  isignalFile.open(fileName);

  if (!isignalFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid signal from " << fileName << endl;
    return;
  }

  // parsing elements to vector
  string line;
  getline(isignalFile, line);
  stringstream ss(line);
  vector<double> vect_elements;
  // check first line if there's starting index
  double temp;
  if (ss >> temp)
  { // if first element is double
    vect_elements.push_back(temp);
    if (ss >> temp)
    { // if there's another double
      // set startIndex
      startIndex = vect_elements[0];
      vect_elements[0] = temp;
    }
    else
    {
      startIndex = 0;
    }
  }
  else
  { // if nonfloat is encountered
    cout << "Encountered nonfloat as first element in " << fileName
         << "\nNo elements imported.";
  }
  // for succeeding numbers
  while (getline(isignalFile, line))
  {
    stringstream ss(line);
    if (ss >> temp)
    {
      vect_elements.push_back(temp);
    }
    else
    { // when encountering nonfloat or eof
      cout << "Parsing of input from " << fileName 
      << " stopped at duration " << vect_elements.size() 
      << " due to nonfloat/new line." << endl;
      break;
    }
  }

  // set duration
  duration = vect_elements.size();

  // compute endIndex
  endIndex = duration + startIndex + 1;

  // convert vector to array
  // allocate memory
  raw = new double[duration];
  for (int i = 0; i < duration; i++)
  {
    raw[i] = vect_elements[i];
  }

  // successful import feedback
  cout << "Signal with start index " << startIndex 
  << ", duration " << duration << ", imported from " 
  << fileName << endl;

  isignalFile.close();
}

void Signal::SignalExport(string fileName)
{
  // creates or open, then checks the file
  ofstream osignalFile;
  osignalFile.open(fileName);
  if (!osignalFile.is_open())
  { // fail export feedback
    cout << "Unable to export signal with start index " 
    << startIndex << ", duration " << duration << ", to " 
    << fileName << endl;
    return;
  }

  // prints signal into the txt file
  osignalFile << startIndex << " ";
  for (int i = 0; i < duration; i++)
  {
    osignalFile << normXcorrData[i] << "\n";
  }
  osignalFile.close();

  // successful exporting feedvack
  cout << "Normalized Crosscorrelation signal with start index " 
  << startIndex << ", duration " << duration << ", exported to " 
  << fileName << endl;
  cout << endl;
}

void Signal::SignalcmdPrint()
{
  if(duration<=20)
  {
    cout << "Normalized Crosscorrelation: " << endl;
    for (int i = 0; i < duration; i++)
    {
      cout << "p_xy(" << i + startIndex << ") = " 
      << normXcorrData[i] << endl;
    }
    cout << endl;
  }
}

void Signal::SignalData()
{
  double Sum = 0;
  for (int i = 0; i < duration; i++)
  {
    Sum += raw[i];
  }
  
  double Average = Sum/duration;
  for (int i = 0; i < duration; i++)
  {
    data[i] = raw[i] - Average;
  }
}

double Signal::computeXcorr(Signal x, Signal y, int lag)
{
  int xcorrEnd;
  int xcorrStart;
  double sum = 0;

  y.startIndex += lag;
  y.endIndex += lag;

  if (x.endIndex < y.endIndex)
  {
    xcorrEnd = x.endIndex;
  }
  else
  {
    xcorrEnd = y.endIndex;
  }

  if (x.startIndex < y.startIndex)
  {
    xcorrStart = x.startIndex;
  }
  else
  {
    xcorrStart = y.startIndex;
  }

  int xcorrDuration = xcorrEnd - xcorrStart - 1;

  int i = 0;
  int j = 0;

  while (i < xcorrDuration && j < xcorrDuration)
  {
    if (x.startIndex != y.startIndex)
    {
      if (x.startIndex > y.startIndex)
      {
        j++;
        y.startIndex++;
      }
      else if (x.startIndex < y.startIndex)
      {
        i++;
        x.startIndex++;
      }
    }
    else
    {
      sum += x.data[i] * y.data[j];
      i++;
      j++;
    }
  }

  return sum; // total sum is basically r_xy
}

Signal Signal::listXcorr(Signal x, Signal y) {
    Signal result;
    result.normXcorrData = new double[duration];
    result.normXcorrData[0] = 0;
    result.startIndex = -(y.duration - 1);
    result.endIndex = x.duration - 1;
    result.duration = endIndex - startIndex + 1;

    for (int i = 0; i < result.duration; i++) {
        // Compute cross-correlation for the signals x and y
        double xcorr_xy = computeXcorr(x, y, i + result.startIndex);
        double xcorr_xx = computeXcorr(x, x, 0);
        double xcorr_yy = computeXcorr(y, y, 0);
        result.normXcorrData[i] = xcorr_xy / sqrt(xcorr_xx * xcorr_yy);
    }

    return result;
}