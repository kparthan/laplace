#ifndef LAPLACE_GENERATOR_H
#define LAPLACE_GENERATOR_H

#include "Support.h"

class DataGenerator
{
  public:
    //! Constructor
    DataGenerator(struct Parameters &, bool);

    //! Sort the elements in the list
    vector<double> sort(vector<double> &);

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

    //! MML estimates
    void mmlEstimate();

  protected:
    //! Choose Normal/Laplace
    bool distribution;

    //! Number of samples
    int samples;

    //! Parameters of simulation
    double mu,b,sigma,noise_sigma;

    //! Random samples generated
    vector<double> x,fx,y;

    //! Quciksort algorithm
    void quicksort(vector<double> &, vector<int> &, int, int);

    //! Partition function used in quicksort()
    int partition(vector<double> &, vector<int> &, int, int);

};

#endif

