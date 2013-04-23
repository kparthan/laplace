#include "NormalDataGenerator.h"

/*!
 *  \brief This is a null constructor module.
 */
NormalDataGenerator::NormalDataGenerator()
{
  type = DataGenerator::NORMAL;
  normal = Normal();
}

/*!
 *  \brief This is a constructor function.
 *  \param mean a double
 *  \param sigma a double
 */
NormalDataGenerator::NormalDataGenerator(double mean, double sigma) 
{
  type = DataGenerator::NORMAL;
  normal = Normal(mean,sigma);
}

/*!
 *  \brief This is a constructor function.
 *  \param p a reference to a struct Parameters
 */
NormalDataGenerator::NormalDataGenerator(struct Parameters &p)
{
  type = DataGenerator::NORMAL;
  parameters = p;
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
vector<double> NormalDataGenerator::generateRandom(int samples)
{
  double mu = normal.mean();
  double sigma = normal.standardDeviation();
  vector<double> x(samples,0);
  //srand(time(NULL));
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
 *  \brief This function simulates the model to generate data using
/bin/bash: Generates: command not found
 */
void NormalDataGenerator::simulate()
{
  for (int i=0; i<parameters.samples.size(); i++) {
    for (int j=0; j<parameters.scale.size(); j++) {
      for (int k=0; k<parameters.noise_sigma.size(); k++) {
        double mean = parameters.mean;
        double scale = parameters.scale[j];
        normal = Normal(mean,scale);
        vector<double> list = generateRandom(parameters.samples[i]);
        xvals = sort(list);
        computeFunctionValues();
        addNoise(parameters.noise_sigma[k]);
        //plotData();
        //mmlEstimate();
      }
    }
  }
}

/*!
 *  This function computes the distribution values for the randomly generated
 *  x values.
 */
void NormalDataGenerator::computeFunctionValues()
{
  int num_samples = xvals.size();
  fxvals = vector<double>(num_samples,0);
  for (int i=0; i<num_samples; i++) {
    fxvals[i] = normal.value(xvals[i]);
  }
}

