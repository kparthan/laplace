#ifndef LAPLACE_GENERATOR_H
#define LAPLACE_GENERATOR_H

#include "Support.h"

class DataGenerator
{
  public:
    //! Null constructor
    DataGenerator();

    //! Constructor
    DataGenerator(struct Parameters &);

    //! Types of distributions
    enum DistributionType {
      DEFAULT,
      NORMAL,
      LAPLACE
    };

    //! Generate x values
    virtual vector<double> generateRandom(){}

    //! Sort the elements in the list
    vector<double> sort(vector<double> &);

    //! Generate data samples
    void generateData();

    //! Compute the corresponding function values
    vector<double> functionValues(vector<double> &);

    //! Add noise to the data
    vector<double> addNoise(vector<double> &);

    //! Plot the data
    void plotData(); 

    //! MML estimates
    void mmlEstimate();

  protected:
    //! Distribution type
    DistributionType type;

    //! Model parameters
    struct Parameters parameters;

    //! Random samples generated
    vector<double> x,fx,y;

    //! Quciksort algorithm
    void quicksort(vector<double> &, vector<int> &, int, int);

    //! Partition function used in quicksort()
    int partition(vector<double> &, vector<int> &, int, int);

};

#endif

