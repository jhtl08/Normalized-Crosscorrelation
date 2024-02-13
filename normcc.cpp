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
  data = new double[1];
  data[0] = 0.0;

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
  data = new double[duration];
  for (int i = 0; i < duration; i++)
  {
    data[i] = vect_elements[i];
  }

  double Sum = 0.0;
  for (int i = 0; i < duration; i++)
  {
    Sum += data[i];
  }
  
  double Average = Sum/duration;
  for (int i = 0; i < duration; i++)
  {
    data[i] = data[i] - Average;
  }

  // successful import feedback
  cout << "Signal with start index " << startIndex 
  << ", duration " << duration << ", imported from " 
  << fileName << endl;

  isignalFile.close();
}

void Signal::SignalExport(string fileName)
{
  if(duration<=20)
  {
    cout << "Normalized Crosscorrelation: " << endl;
    for (int i = 0; i < duration; i++)
    {
      cout << "p_xy(" << i + startIndex << ") = " 
      << data[i] << endl;
    }
    cout << endl;
  }

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
    osignalFile << data[i] << "\n";
  }
  osignalFile.close();

  // successful exporting feedvack
  cout << "Normalized Crosscorrelation signal with start index " 
  << startIndex << ", duration " << duration << ", exported to " 
  << fileName << endl;
  cout << endl;
}

double Signal::computeXcorr(Signal x, Signal y, int lag)
{
double sum = 0.0;

// Move start and end indices by lag for y
y.startIndex += lag;
y.endIndex += lag;

int i = 0; // Index for x
int j = 0; // Index for y

while (x.startIndex < x.endIndex && y.startIndex < y.endIndex && i < x.duration && j < y.duration)
{
    if (x.startIndex != y.startIndex)
    {
        // Increment the index for the array with the smaller start index
        if (x.startIndex > y.startIndex)
        {
            j++; // Increment index for y
            y.startIndex++; // Move y's start index
        }
        else // x.startIndex < y.startIndex
        {
            i++; // Increment index for x
            x.startIndex++; // Move x's start index
        }
    }
    else // x.startIndex == y.startIndex
    {
        // Add the product to the sum
        sum += x.data[i] * y.data[j];

        // Move indices for both x and y
        i++;
        j++;
        x.startIndex++;
        y.startIndex++;
    }
}

return sum; // total sum is basically r_xy
}

Signal Signal::normalizedXCorr(Signal x, Signal y) {
  
    Signal result;

    int diff = abs(x.startIndex - y.startIndex);

    if (y.startIndex < x.startIndex)
    {
      diff = -diff;
    }

    result.startIndex = -(y.duration - 1 + diff) ;
    result.duration = y.duration + x.duration - 1;
    result.endIndex = result.duration + result.startIndex + 1;
    result.data = new double[result.duration];
    result.data[0] = 0;

    for (int i = 0; i < result.duration; i++) {
        // Compute cross-correlation for the signals x and y
        double xcorr_xy = computeXcorr(x, y, i + result.startIndex);
        double xcorr_xx = computeXcorr(x, x, 0);
        double xcorr_yy = computeXcorr(y, y, 0);
        result.data[i] = xcorr_xy / sqrt(xcorr_xx * xcorr_yy);
    }

    return result;
}

