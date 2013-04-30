#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int print;
  vector<string> pdb_files;
  int iterations;
  double acceptance;
  double step;
};

struct Parameters parseCommandLineInput(int, char **);
void Usage(const char *, options_description &);
bool checkFile(const char *);
ProteinStructure* parsePDBFile(const char *);
double computeMean(vector<double> &);
double computeMedian(vector<double> &);

#endif

