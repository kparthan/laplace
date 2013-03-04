#ifndef SUPPORT_H
#define SUPPORT_H

#include "Header.h"

struct Parameters
{
  int samples;
  double mu;
  double b;
  double noise_sigma;
};

struct Parameters parseCommandLineInput (int, char **);
void Usage (const char *, options_description &);
void laplaceFit(struct Parameters &);

#endif

