#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  vector<int> samples;
  double mean;
  vector<double> scale;
  vector<double> noise_sigma;
  string distribution;
  string estimate;
};

struct Parameters parseCommandLineInput(int, char **);
void Usage(const char *, options_description &);
int sign(double);
void fitData(struct Parameters &);
pair<double,double> extremum(vector<double> &);
double minimum(vector<double> &);
double maximum(vector<double> &);
void printError(const char *);
double computeMean(vector<double> &);
double computeMedian(vector<double> &);

#endif

