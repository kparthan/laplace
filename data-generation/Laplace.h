#ifndef LAPLACE_H
#define LAPLACE_H

#include "Header.h"

class Laplace
{
  private:
    //! Mean of the distribution
    double mu;

    //! Scale parameter
    double b;

  public:
    //! Null constructor
    Laplace();

    //! Constructor
    Laplace(double, double);

    //! Overloading = operator
    Laplace operator=(const Laplace &);

    //! Return the mean of the distribution
	  const double mean();
	
	  //! Return the scale parameter of the distribution
	  const double scaleParameter();

    //! Value of the mathematical function at a given x
    double value(double);

};

#endif

