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

bool Signal::SignalImport(string fileName)
{
  // opens and checks the files
  ifstream isignalFile;
  isignalFile.open(fileName);
  if (!isignalFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid signal from " 
    << fileName << endl;
    return false;
  }
  // parsing elements to vector
  string line;
  getline(isignalFile, line);
  stringstream ss(line);

  vector<double> vect_elements;

  double temp;
  if (ss >> temp)
  { // if first element is double
    vect_elements.push_back(temp);
    if (ss >> temp)
    {
      if (ss.eof())
      {
        startIndex = vect_elements[0];
        vect_elements[0] = temp;
      }
      else
      {
        cout << "1Encountered nonfloat as first element in " << fileName
        << "\nNo elements imported.\n" << endl;
        return false;
      }
    }
    else
    {
      if (ss.eof())
      {
        startIndex = 0;
      }
      else
      {
        cout << "2Encountered nonfloat as first element in " << fileName
        << "\nNo elements imported.\n" << endl;
        return false;
      }
    }
  }
  else
  { // if nonfloat is encountered
    cout << "3Encountered nonfloat as first element in " << fileName
    << "\nNo elements imported.\n" << endl;
    return false;
  }
  
  string word;
  bool valid = true;

  while (getline(isignalFile, line) && valid) {
    stringstream ss(line);
    while (ss >> word)
    {
      stringstream ss2(word);
      if (ss2 >> temp)
      {
        if (ss2.eof())
        {
          vect_elements.push_back(temp);
        }
        else
        {
          valid = false;
          break;
        }
      }
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

  // compute for the sum
  double Sum = 0.0;
  for (int i = 0; i < duration; i++)
  {
    Sum += data[i]; 
  }
  
  // compute for the average
  double Average = Sum/duration;

  // compute for the actual value
  for (int i = 0; i < duration; i++)
  {
    data[i] -= Average;
  }

  // successful import feedback
  cout << "Signal with start index " << startIndex 
  << ", duration " << duration << ", imported from " 
  << fileName << "\n" << endl;

  isignalFile.close();
  return true;
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
  return;
}

double Signal::computeXcorr(Signal x, Signal y, int lag) 
{
  double sum = 0.0; // Initialize sum

  // Move start and end indices of y by lag
  y.startIndex += lag;
  y.endIndex += lag;

  // Determine the index for each signal
  int indexX = 0; 
  int indexY = 0; 

  // Compute the difference between starting indices
  int startIndexDiff = abs(x.startIndex - y.startIndex);

  // Adjust starting indices to align signals
  if (x.startIndex < y.startIndex) 
  {
    indexX += startIndexDiff;
  }
  if (y.startIndex < x.startIndex) 
  {
    indexY += startIndexDiff;
  }

  // Compute for the cross correlation
  for (; indexX < x.duration && indexY < y.duration; 
  indexX++, indexY++)
  {
    // Add product of signal values to sum
    sum += x.data[indexX] * y.data[indexY];
  }

  return sum; // Return cross-correlation
}

Signal Signal::normalizedXCorr(Signal x, Signal y) 
{
  Signal result;

  // Compute for the difference between the starting index of x and y
  int diff = x.startIndex - y.startIndex;
  result.startIndex = -(y.duration - 1 - diff);

  // Compute for the duration (using shift left and shift right)
  result.duration = y.duration + x.duration - 1;

  // Compute for the end index
  result.endIndex = result.duration + result.startIndex + 1;

  // Allocating new memory
  result.data = new double[result.duration];
  result.data[0] = 0.0;

  // Compute for the normalized cross-correlation 
  // for all applicable l (Lag)
  for (int i = 0; i < result.duration; i++) 
  {
    // Compute cross-correlation for the signals x and y
    double xcorr_xy = computeXcorr(x, y, i + result.startIndex);

    // Compute cross-correlation for the signals x and x
    double xcorr_xx = computeXcorr(x, x, 0);

    // Compute cross-correlation for the signals y and y
    double xcorr_yy = computeXcorr(y, y, 0);

    // Compute normalized cross-correlation
    result.data[i] = xcorr_xy / sqrt(xcorr_xx * xcorr_yy);
  }

  return result;
}

