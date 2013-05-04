#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  vector<int> samples;
  double mean;
  vector<double> scale;
  //vector<double> noise_sigma;
  string distribution;
  string estimate;
  int iterations;
};

struct MML_Estimates
{
  double normal_mean;
  double normal_sigma;
  double normal_mml;
  double laplace_mean;
  double laplace_scale;
  double laplace_mml;
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

