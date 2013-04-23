#include "Laplace.h"

/*!
 *  \brief This is a null constructor module.
 */
Laplace::Laplace() : mu(0), b(1)
{}


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
    mu = source.mu;
    b = source.b;
  }
  return *this;
}

/*!
 *  \brief This function returns the mean of the distribution
 *  \return the mean of the distribution
 */
const double Laplace::mean(void)
{
	return mu;
}

/*!
 *  \brief This function returns the scale parameter of the distribution
 *  \return the scale parameter of the distribution
 */
const double Laplace::scaleParameter(void)
{
	return b;
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

