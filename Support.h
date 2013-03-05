#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int samples;
  double mu;
  double scale;
  double noise_sigma;
};

struct Parameters parseCommandLineInput(int, char **);
void Usage(const char *, options_description &);
int sign(double);
void laplaceFit(struct Parameters &);
pair<double,double> extremum(vector<double> &);
double minimum(vector<double> &);
double maximum(vector<double> &);

#endif

