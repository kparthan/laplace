#ifndef LAPLACE_H
#define LAPLACE_H

#include "Support.h"

class Laplace
{
  private:
    //! Mean of the distribution
    double mu;

    //! Scale parameter
    double b;

  public:
    //! Constructor
    Laplace(double, double);

	//! Return the mean of the distribution
	const double mean();
	
	//! Return the scale parameter of the distribution
	const double scaleParameter();

    //! Overloading = operator
    Laplace operator=(const Laplace &);

    //! Value of the mathematical function at a given x
    double value(double);

    //! Generates samples from a Laplace distribution
    vector<double> generate(int);
};

#endif

