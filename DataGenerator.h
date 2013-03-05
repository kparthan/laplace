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
    DataGenerator(struct Parameters &, bool);

    //! Generate data samples
    void generateData();

    //! Generate x values
    vector<double> generateRandom();

    //! Compute the corresponding function values
    vector<double> functionValues(vector<double> &);

    //! Add noise to the data
    vector<double> addNoise(vector<double> &);

    //! Plot the data
    void plotData(); 
};

#endif

