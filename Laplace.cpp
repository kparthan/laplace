#include "Laplace.h"

/*!
 *  \brief Constructor module
 *  \param mu a double
 *  \param b a double
 */
Laplace::Laplace(double mu, double b) : mu(mu), b(b)
{}

/*!
 *  \brief This function assigns a copy of Laplace to the source
 *  \param source a reference to a Laplace object
 *  \return a Laplace object
 */
Laplace Laplace::operator=(const Laplace &source)
{
  if (this != &source) {
    n = source.n;
    noise_sigma = source.noise_sigma;
    pdf = source.pdf;
  }
  return *this;
}

/*!
 *  \brief This function computes the value at a given x
 *  \param x a double
 *  \return the function value at x
 */
double Laplace::value(double x)
{
  double dev = fabs(x - mu);
  double exponent = -dev / (double)b;
  return exp(exponent/(2*b));
}

