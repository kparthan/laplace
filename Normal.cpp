#include "Normal.h"

/*!
 *  \brief This is a constructor module
 *  sets default values of mean as 0 and standard deviation as 1
 */
Normal::Normal() : mu(0), sigma(1)
{}

/*!
 *  \brief constructor function which sets the value of mean and 
 *  standard deviation of the distribution
 *  \param mu a double
 *  \param sigma a double
 */
Normal::Normal(double mu, double sigma) : mu(mu), sigma(sigma)
{}

/*!
 *  \brief This function computes the value of the distribution at a given x
 *  \param x a double
 *  \return value of the function given x
 */
double Normal::value(double x)
{
	double expNumerator = (-1) * (x-mu) * (x-mu);
	double expDenominator = 2 * sigma * sigma;
	return (exp (expNumerator/expDenominator)) / ((sqrt(2*PI)) * sigma);
}

/*!
 *  \brief This function generates a data point sampled from this 
 *	Normal distribution. Uses Box-Muller method to draw samples from the 
 *	standard normal distribution i.e., N(0,1)
 *	X = sqrt(-2 ln U) cos(2*pi*V)
 *	Y = sqrt(-2 ln U) sin(2*pi*V), where
 *	U,V are drawn from a uniform distribution in (0,1). The resultant X,Y
 *	will be sampled from a standard normal distribution
 *	To generate from a general N(mu,sigma), use the transformation:
 *	Z = mu + sigma * X, where X~N(0,1)
 *  \param samples an integer
 *	\return a sample drawn from the normal distribution
 */
vector<double> Normal::generate(int samples)
{
  vector<double> x(samples,0);
  srand(1000);
  for (int i=0; i<samples; i=i+2) {
    double u = (double) rand() / RAND_MAX;
    double v = (double) rand() / RAND_MAX;
    double sqroot = sqrt(-2 * log(u));
    double arg = 2 * PI * v;
    double r1 = sqroot * cos (arg);
    double r2 = sqroot * sin (arg);
	  x[i] = mu + sigma * r1;
    if (i != samples-1) {
	    x[i+1] = mu + sigma * r2;
    }
  }
	return x;
}

/*!
 *  \brief This function returns the mean of the distribution
 *  \return the mean of the distribution
 */
const double Normal::mean(void)
{
	return mu;
}

/*!
 *  \brief This function returns the standard deviation of the distribution
 *  \return the standard deviation of the distribution
 */
const double Normal::standardDeviation(void)
{
	return sigma;
}

