#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int print;
  int generate;
  vector<int> samples;
  double mean;
  vector<double> scale;
  string distribution;
  int iterations;
  string data_file;
  double aom;
};

struct Estimates
{
  int winner_ml;
  int winner_mml;
  // Normal estimates
  double normal_mean;
  double normal_scale_ml;
  double normal_scale_mml;
  double normal_likelihood;
  double normal_msglen;
  // Laplace estimates
  double laplace_mean;
  double laplace_scale_ml;
  double laplace_scale_mml;
  double laplace_likelihood;
  double laplace_msglen;
};

vector<double> parseFile(string &);
void printEstimates(struct Estimates &);
struct Parameters parseCommandLineInput(int, char **);
void Usage(const char *, options_description &);
int sign(double);
void fitData(struct Parameters &);
pair<double,double> extremum(vector<double> &);
double minimum(vector<double> &);
double maximum(vector<double> &);
void printError(const char *);
double computeMean(vector<double> &);
double computeVariance(vector<double> &);
double computeVariance(vector<double> &, double);
double computeMedian(vector<double> &);
template<class T> string convertToString(T);

string fileName(string, double, int, double, int);
vector<double> getColumn(string, int);
vector<double> getRange(vector<double> &);
void analyzeScale(struct Parameters &);
void analyzeScaleVariance(struct Parameters &);
void analyzeDiffMsglen(struct Parameters &);
void plotScaleBoxplots(int, double, double, string, string, string, string,
                       struct Parameters &);

vector<double> sort(vector<double> &);
void quicksort(vector<double> &, vector<int> &, int, int);
int partition(vector<double> &, vector<int> &, int, int);

#endif

