#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int print;
  vector<int> samples;
  double mean;
  vector<double> scale;
  string distribution;
  string estimate;
  int iterations;
};

struct Estimates
{
  double normal_mean;
  double normal_sigma_ml;
  double normal_sigma_mml;
  double normal_msglen;
  double laplace_mean;
  double laplace_scale_ml;
  double laplace_scale_mml;
  double laplace_msglen;
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
template<class T> string convertToString(T);

#endif

