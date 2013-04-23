#ifndef NORMAL_DATA_GENERATOR_H
#define NORMAL_DATA_GENERATOR_H

#include "Header.h"
#include "DataGenerator.h"
#include "Normal.h"

class NormalDataGenerator : public DataGenerator
{
  private:
    // An instance of a Normal distribution
    Normal normal;

	public:
    //! Null constructor
    NormalDataGenerator();

    //! Constructor
    NormalDataGenerator(double, double);

    //! Constructor
    NormalDataGenerator(struct Parameters &);

    //! Simulate the experiment
    void simulate();

    //! Generate random x values from a Normal distribution
    vector<double> generateRandom(int);

    //! Compute the function value for each sample
    void computeFunctionValues();

};

#endif

