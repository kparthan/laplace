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
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  srand(now.tv_nsec);
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
      vector<double> list;  
      int outcomes[3] = {0};
      for(int n=1; n<=parameters.iterations; n++) {
        double mean = parameters.mean;
        double scale = parameters.scale[j];
        laplace = Laplace(mean,scale);
        list = generateRandom(parameters.samples[i]);
        Estimates estimates = estimateAndPlotModel("laplace",list,j,n);
        int result = estimateAndPlotModel("laplace",list,j,n);
        outcomes[result]++;
      }
      cout << "N:L:D = " << outcomes[0] << ":";
      cout << outcomes[1] << ":" << outcomes[2] << endl;
      if (parameters.iterations > 1) {
        plotStatistics("laplace",list.size(),j);
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

