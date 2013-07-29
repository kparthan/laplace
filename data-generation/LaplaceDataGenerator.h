#ifndef LAPLACE_DATA_GENERATOR_H
#define LAPLACE_DATA_GENERATOR_H

#include "Support.h"
#include "DataGenerator.h"
#include "Laplace.h"

class LaplaceDataGenerator : public DataGenerator
{
  private:
    //! An instance of a Laplace distribution
    Laplace laplace;

  public:
    //! Null constructor
    LaplaceDataGenerator();

    //! Constructor
    LaplaceDataGenerator(double, double);

    //! Constructor
    LaplaceDataGenerator(struct Parameters &);

    //! Simulate the experiment
    void simulate();

    struct Estimates simulate2(int, double, double);

    //! Generate random x values from a Normal distribution
    vector<double> generateRandom(int);

    //! Compute the function value for each sample
    vector<double> computeFunctionValues(vector<double> &);
};

#endif

