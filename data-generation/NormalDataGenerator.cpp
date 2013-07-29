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
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  srand(now.tv_nsec);
  double r1,r2;
  unsigned t = now.tv_nsec;
  int y = 100;
  for (int i=0; i<samples; i++) {
    srand(t);
    t += 100;
    double u = rand() / (double)RAND_MAX;
    //cout << "u: " << u << endl;

    srand(t);
    t += 100;
    double v = rand() / (double)RAND_MAX;
    //cout << "v: " << v << endl;

    double sqroot = sqrt(-2 * log(u));
    double arg = 2 * PI * v;
    r1 = sqroot * cos (arg);
    //r2 = sqroot * sin (arg);
	  x[i] = mu + sigma * r1;
    //cout << "x: " << x[i] << endl;
    /*if (i != samples-1) {
	    x[i+1] = mu + sigma * r2;
    }*/
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
 *  Normal distribution.
 */
void NormalDataGenerator::simulate()
{
  for (int i=0; i<parameters.samples.size(); i++) {
    for (int j=0; j<parameters.scale.size(); j++) {
      vector<double> list;
      int mml_outcomes[3] = {0};
      int ml_outcomes[3] = {0};
      for (int n=1; n<=parameters.iterations; n++) {
        double mean = parameters.mean;
        double scale = parameters.scale[j];
        normal = Normal(mean,scale);
        vector<double> data = generateRandom(parameters.samples[i]);
        list = modifyDataToAOM(data,parameters.aom);
        struct Estimates estimates = estimateAndPlotModel("normal",list,j);
        mml_outcomes[estimates.winner_mml]++;
        ml_outcomes[estimates.winner_ml]++;
      }
      cout << "N:L:D  [ML] = " << ml_outcomes[0] << ":";
      cout << ml_outcomes[1] << ":" << ml_outcomes[2] << " -- ";
      cout << " [MML] = " << mml_outcomes[0] << ":";
      cout << mml_outcomes[1] << ":" << mml_outcomes[2] << endl;
    }
  }
}

/*!
 *  This function computes the distribution values for the randomly generated
 *  x values.
 *  \param x a reference to a vector<double>
 *  \return the function values
 */
vector<double> NormalDataGenerator::computeFunctionValues(vector<double> &x)
{
  int num_samples = x.size();
  fx = vector<double>(num_samples,0);
  for (int i=0; i<num_samples; i++) {
    fx[i] = normal.value(x[i]);
  }
  return fx;
}

struct Estimates NormalDataGenerator::simulate2(int n, double m, double b)
{
  normal = Normal(m,b);
  //cout << "Normal Gen\n";
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

