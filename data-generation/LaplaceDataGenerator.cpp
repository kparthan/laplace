#include "LaplaceDataGenerator.h"

/*!
 *  \brief This is a null constructor module.
 */
LaplaceDataGenerator::LaplaceDataGenerator()
{
  type = DataGenerator::LAPLACE;
  laplace = Laplace();
}

/*!
 *  \brief This is a constructor function
 *  \param mean a double
 *  \param scale a double
 */
LaplaceDataGenerator::LaplaceDataGenerator(double mean, double scale)
{
  type = DataGenerator::LAPLACE;
  laplace = Laplace(mean,scale);
}

/*!
 *  \brief This is a constructor function
 *  \param p a reference to a struct Parameters
 */
LaplaceDataGenerator::LaplaceDataGenerator(struct Parameters &p)
{
  type = DataGenerator::LAPLACE;
  parameters = p;
}

/*!
 *  \brief This function generates samples from a Laplace distribution
 *  \param samples an integer
 *  \return the list of samples generated
 */
vector<double> LaplaceDataGenerator::generateRandom(int samples)
{
  double mu = laplace.mean();
  double b = laplace.scaleParameter();
  vector<double> x(samples,0);
  srand(1000);
  for (int i=0; i<samples; i++) {
    double random = (double)rand() / RAND_MAX;
    random -= 0.5;
    x[i] = mu - b * sign(random) * log(1-2*fabs(random));
  }
  return x;
}

/*!
 *  \brief This function simulates the model to generate data using
 *  Laplace distribution.
 */
void LaplaceDataGenerator::simulate()
{
  for (int i=0; i<parameters.samples.size(); i++) {
    for (int j=0; j<parameters.scale.size(); j++) {
      for (int k=0; k<parameters.noise_sigma.size(); k++) {
        double mean = parameters.mean;
        double scale = parameters.scale[j];
        laplace = Laplace(mean,scale);
        vector<double> list = generateRandom(parameters.samples[i]);
        estimateAndPlotModel("laplace",list,j,k);
      }
    }
  }
}

/*!
 *  This function computes the distribution values for the randomly generated
 *  x values.
 *  \param x a reference to a vector<double>
 *  \return the function values
 */
vector<double> LaplaceDataGenerator::computeFunctionValues(vector<double> &x)
{
  int num_samples = x.size();
  fx = vector<double>(num_samples,0);
  for (int i=0; i<num_samples; i++) {
    fx[i] = laplace.value(x[i]);
  }
  return fx;
}
