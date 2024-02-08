// normcc.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// February 5, 2022

#include "normcc.h"
#include <sstream>

using namespace std;

Signal::Signal()
{
  startIndex = 0;
  duration = 1;
  endIndex = 0;
  elements = new double[1];
  elements[0] = 0;
}

Signal::Signal(double *signalArray, int inputStart,
               int inputDuration)
{
  startIndex = inputStart;
  duration = inputDuration;
  endIndex = duration + startIndex + 1;
  elements = new double[duration];
  for (int i = 0; i < duration; i++)
  {
    elements[i] = signalArray[i];
  }
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
      cout << "Parsing of input from " << fileName << " stopped at duration " << vect_elements.size() << " due to nonfloat/new line." << endl;
      break;
    }
  }

  // set duration
  duration = vect_elements.size();

  // compute endIndex
  endIndex = duration + startIndex + 1;

  // convert vector to array
  // allocate memory
  elements = new double[duration];
  for (int i = 0; i < duration; i++)
  {
    elements[i] = vect_elements[i];
  }

  // successful import feedback
  cout << "Signal with start index " << startIndex << ", duration " << duration << ", imported from " << fileName << endl;

  isignalFile.close();
}

void Signal::SignalExport(string fileName)
{
  // creates or open, then checks the file
  ofstream osignalFile;
  osignalFile.open(fileName);
  if (!osignalFile.is_open())
  { // fail export feedback
    cout << "Unable to export signal with start index " << startIndex << ", duration " << duration << ", to " << fileName << endl;
    return;
  }

  // prints signal into the txt file
  osignalFile << startIndex << " ";
  for (int i = 0; i < duration; i++)
  {
    osignalFile << elements[i] << "\n";
  }
  osignalFile.close();

  // successful exporting feedvack
  cout << "Crosscorrelation signal with start index " << startIndex << ", duration " << duration << ", exported to " << fileName << endl;
}

double Signal::computeXcorr(Signal x, Signal y, int lag)
{
  int xcorrEnd;
  int xcorrStart;
  int sum = 0;

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

  int xcorrDuration = xcorrEnd - xcorrStart + 1;

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
      sum += x.elements[i] * y.elements[j];
      i++;
      j++;
    }
  }

  return sum; // total sum is basically r_xy
}

Signal Signal::listXcorr(Signal x, Signal y)
{
  int lastEndIndex;
  int lagdependentStarty;

  lagdependentStarty = x.startIndex - y.duration;
  lastEndIndex = x.endIndex + y.duration;

  cout << "y =" << lagdependentStarty << endl;

  int r_xyDuration = lastEndIndex - lagdependentStarty + 1;

  // assign output attributes
  duration = r_xyDuration;
  startIndex = lagdependentStarty;
  endIndex = lastEndIndex;

  cout << "x.endIndex = " << x.endIndex << endl;
  cout << "lagdepstarty = " << lagdependentStarty << endl;
  cout << "r duration = " << r_xyDuration << endl;

  elements = new double[r_xyDuration];
  int i = 0;
  for (lagdependentStarty; lagdependentStarty <= lastEndIndex;
       lagdependentStarty++)
  {
    elements[i] =
        computeXcorr(x, y, lagdependentStarty);
    i++;
    cout << "y = " << lagdependentStarty << " e[i] = " << elements[i] << " xcomputed = " << computeXcorr(x, y, lagdependentStarty) << endl;
  }
}