#ifndef LAPLACE_GENERATOR_H
#define LAPLACE_GENERATOR_H

#include "Support.h"
#include "Laplace.h"

class LaplaceGenerator
{
  private:
    //! Number of samples
    int n;

    //! Standard deviation of Gaussian noise added
    double noise_sigma;

    //! The Laplace distribution
    Laplace pdf;

    //! Random samples generated
    vector<double> x,fx,y;

  public:
    //! Constructor
    LaplaceGenerator(struct Parameters &);

    //! Generate data samples
    vector<double> generate();
};

#endif

