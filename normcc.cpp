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

Signal Signal::listXcorr(Signal x, Signal y) {
    // Normalize signals
    double xSum = 0;
    for (int c = 0; c < x.duration; c++)
    {
      xSum += x.elements[c];
    }
    double xAverage;
    xAverage = xSum/x.duration;

    double ySum = 0;
    for (int c = 0; c < y.duration; c++)
    {
      ySum += y.elements[c];
    }
    double yAverage;
    yAverage = ySum/y.duration;

    for (int t = 0; t < x.duration; t++)
    {
      x.elements[t] = x.elements[t] - xAverage;
    }

    for (int t = 0; t < y.duration; t++)
    {
      y.elements[t] = y.elements[t] - yAverage;
    }

    int startIndex = -(y.duration - 1);
    cout << "Start Index: " << startIndex << endl;

    int endIndex = x.duration - 1;
    cout << "End Index: " << endIndex << endl;

    int duration = endIndex - startIndex + 1;
    cout << "Duration: " << duration << endl;


    // Create a new Signal object to store the cross-correlation results
    Signal result;
    result.elements = new double[duration];
    result.startIndex = startIndex;
    result.duration = duration;
    result.endIndex = endIndex;

    for (int i = 0; i < duration; i++) {
        // Compute cross-correlation for the original signals x and y
        result.elements[i] = computeXcorr(x, y, i + startIndex)/sqrt(computeXcorr(x, x, 0)*computeXcorr(y, y, 0));
    }

    return result;
}



