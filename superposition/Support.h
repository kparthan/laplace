#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int print;
  vector<string> pdb_files;
  int iterations;
  double acceptance;
  double increment_translation;
  double increment_rotation_angle;
  int encode_deviations;
  int estimate_method;
};

struct Parameters parseCommandLineInput(int, char **);
void Usage(const char *, options_description &);
bool checkFile(const char *);
ProteinStructure* parsePDBFile(const char *);
double computeMean(vector<double> &);
double computeMedian(vector<double> &);
vector<double> sort(vector<double> &);
void quicksort(vector<double> &, vector<int> &, int, int);
int partition(vector<double> &, vector<int> &, int, int);

#endif

