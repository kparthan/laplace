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

/*struct Statistics
{
  vector<double> normal_scale;
    //normal_scale[0] avg_normal_scale_ml;
    //normal_scale[1] min_normal_scale_ml;
    //normal_scale[2] max_normal_scale_ml;
    //normal_scale[3] avg_normal_scale_mml;
    //normal_scale[4] min_normal_scale_mml;
    //normal_scale[5] max_normal_scale_mml;
  vector<double> laplace_scale;
    //laplace_sigma[0] avg_laplace_sigma_ml;
    //laplace_sigma[1] min_laplace_sigma_ml;
    //laplace_sigma[2] max_laplace_sigma_ml;
    //laplace_sigma[3] avg_laplace_sigma_mml;
    //laplace_sigma[4] min_laplace_sigma_mml;
    //laplace_sigma[5] max_laplace_sigma_mml;
};*/

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
double computeMedian(vector<double> &);
template<class T> string convertToString(T);

string fileName(string, double, int, double, int);
void analyzeScale(struct Parameters &);
void analyzeVarianceScale(struct Parameters &);
void analyzeDiffMsglen(struct Parameters &);

#endif

