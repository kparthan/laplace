#ifndef LAPLACE_GENERATOR_H
#define LAPLACE_GENERATOR_H

#include "Support.h"

class DataGenerator
{
  private:
    //! Choose Normal/Laplace
    bool distribution;

    //! Number of samples
    int samples;

    //! Parameters of simulation
    double mu,b,sigma,noise_sigma;

    //! Random samples generated
    vector<double> x,fx,y;

  public:
    //! Constructor
    DataGenerator(struct Parameters &);

    //! Generate data samples
    generateData();

    //! Generate x values
    vector<double> generateRandom();

    //! Compute the corresponding function values
    vector<double> functionValues();

    //! Add noise to the data
    vector<double> addNoise();

    //! Plot the data
    plotData(); 
};

#endif

