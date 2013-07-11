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
  int winner;
  double normal_mean;
  double normal_sigma_ml;
  double normal_sigma_mml;
  double normal_msglen;
  double laplace_mean;
  double laplace_scale_ml;
  double laplace_scale_mml;
  double laplace_msglen;
};

struct Statistics
{
  vector<double> normal_sigma;
    //normal_sigma[0] avg_normal_sigma_ml;
    //normal_sigma[1] min_normal_sigma_ml;
    //normal_sigma[2] max_normal_sigma_ml;
    //normal_sigma[3] avg_normal_sigma_mml;
    //normal_sigma[4] min_normal_sigma_mml;
    //normal_sigma[5] max_normal_sigma_mml;
  vector<double> laplace_scale;
    //laplace_sigma[0] avg_laplace_sigma_ml;
    //laplace_sigma[1] min_laplace_sigma_ml;
    //laplace_sigma[2] max_laplace_sigma_ml;
    //laplace_sigma[3] avg_laplace_sigma_mml;
    //laplace_sigma[4] min_laplace_sigma_mml;
    //laplace_sigma[5] max_laplace_sigma_mml;
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

