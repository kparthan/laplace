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
  int y = 100;
  for (int i=0; i<samples; i++) {
    double random = uniform_random(); 
    //double random = rand() / (double)RAND_MAX;
    //cout << "random: " << random << endl;
    random -= 0.5;
    x[i] = mu - b * sign(random) * log(1-2*fabs(random));
    //cout << "x: " << x[i] << endl;
    int xtemp = convertNumberAOM(parameters.aom,x[i]);
    if (y == xtemp) {
      i--;
      continue;
    } else {
      y = xtemp;
    }
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
      int mml_outcomes[3] = {0};
      int ml_outcomes[3] = {0};
      for(int n=1; n<=parameters.iterations; n++) {
        double mean = parameters.mean;
        double scale = parameters.scale[j];
        laplace = Laplace(mean,scale);
        vector<double> data = generateRandom(parameters.samples[i]);
        list = modifyDataToAOM(data,parameters.aom);
        struct Estimates estimates = estimateAndPlotModel("laplace",list,j);
        mml_outcomes[estimates.winner_mml]++;
        ml_outcomes[estimates.winner_ml]++;
      }
      //if (parameters.iterations == 1) {
        //plotMessageLength("laplace",parameters.samples[i],j);
      //}
      cout << "N:L:D  [ML] = " << ml_outcomes[0] << ":";
      cout << ml_outcomes[1] << ":" << ml_outcomes[2] << " -- ";
      cout << " [MML] = " << mml_outcomes[0] << ":";
      cout << mml_outcomes[1] << ":" << mml_outcomes[2] << endl;
    }
  }
}

struct Estimates LaplaceDataGenerator::simulate2(int n, double m, double b)
{
  laplace = Laplace(m,b);
  //cout << "Laplace Gen\n";
  vector<double> data = generateRandom(n);
  vector<double> list = modifyDataToAOM(data,parameters.aom);
  vector<double> x = sort(list);
  struct Estimates estimates = parameterEstimation(x,parameters.aom);
  predict(x,estimates);

  vector<double> likelihood = logLikelihood(predictions_ml);
  estimates.normal_likelihood = likelihood[0];
  estimates.laplace_likelihood = likelihood[1];
  if (estimates.normal_likelihood > estimates.laplace_likelihood) {
    estimates.winner_ml = NORMAL;
  } else {
    estimates.winner_ml = LAPLACE;
  }
  predictions_ml[0].clear();predictions_ml[1].clear();predictions_ml.clear();
  predictions_mml[0].clear();predictions_mml[1].clear();predictions_mml.clear();
  return estimates;
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

