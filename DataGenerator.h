#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

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

    //! Simulate the experiment
    virtual void simulate(){}

    //! Generate x values
    virtual vector<double> generateRandom(int){}

    //! Compute the corresponding function values
    virtual void computeFunctionValues(){}

    //! Add noise to the data
    void addNoise(double);

    //! Sort the elements in the list
    vector<double> sort(vector<double> &);

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
    vector<double> xvals,fxvals,yvals;

    //! Quciksort algorithm
    void quicksort(vector<double> &, vector<int> &, int, int);

    //! Partition function used in quicksort()
    int partition(vector<double> &, vector<int> &, int, int);

};

#endif

