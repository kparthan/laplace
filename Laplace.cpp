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
    mu = source.mu;
    b = source.b;
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

/*!
 *  \brief This function generates samples from a Laplace distribution
 *  \param samples an integer
 *  \return the list of samples generated
 */
vector<double> Laplace::generate(int samples)
{
  vector<double> x(samples,0);
  srand(1000);
  for (int i=0; i<samples; i++) {
    double random = (double)rand() / RAND_MAX;
    random -= 0.5;
    x[i] = mu - b * sign(random) * log(1-2*fabs(random));
  }
  return x;
}

