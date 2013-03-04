#include "LaplaceGenerator.h"

/*!
 *  \brief Constructor
 *  \param parameters a reference to a struct Parameters
 */
LaplaceGenerator::LaplaceGenerator(struct Parameters &parameters)
{
  n = parameters.samples;
  pdf = Laplace(parameters.mu,parameters.b);
  noise_sigma = parameters.noise_sigma;
}

/*!
 *  \brief This module generates data samples from a Laplace distribution
 *  \return a set of data samples
 */
vector<double> LaplaceGenerator::generate()
{
  x = generateRandom();
  fx = functionValues();
  y = addNoise();
  return y;
}

/*!
 *  \brief This function generates random data in a range
 *  \return the data generated
 */
vector<double> LaplaceGenerator::generateRandom()
{
  x = vector<double>(n,0);
  srand(1000);
  for (int i=0; i<n; i++) {
    double random = rand() / RAND_MAX;
    random -= 0.5;
  }
}

