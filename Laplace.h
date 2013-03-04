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
    //! Constructor
    Laplace(double, double);

    //! Overloading = operator
    Laplace operator=(const Laplace &);

    //! Value of the mathematical function at a given x
    double value(double);
};

#endif

