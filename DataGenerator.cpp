#include "DataGenerator.h"
#include "Normal.h"
#include "Laplace.h"
#include "Plot.h"

/*!
 *  \brief Constructor
 *  \param parameters a reference to a struct Parameters
 *  \param distribution a boolean
 */
DataGenerator::DataGenerator(struct Parameters &parameters, bool distribution):
                             distribution(distribution),
                             samples(parameters.samples),
                             mu(parameters.mu),
                             noise_sigma(parameters.noise_sigma)
{
  if (distribution == 0) {
    sigma = parameters.scale;
  } else if (distribution == 1) {
    b = parameters.scale;
  }
}

/*!
 *  \brief This module generates data samples from a Data distribution
 */
void DataGenerator::generateData()
{
  generateRandom();
  functionValues(x);
  addNoise(fx);
}

/*!
 *  \brief This function generates random data in a range
 *  \return the random samples generated
 */
vector<double> DataGenerator::generateRandom()
{
  switch(distribution) {
    case 0: // Normal
	{
      Normal pdf(mu,sigma);
      x = pdf.generate(samples);
      break;
	}

    case 1: // Laplace
	{
      Laplace pdf(mu,b);
      x = pdf.generate(samples);
      break;
	}

    default:
      cout << "Distribution not supported ..." << endl;
      exit(1);
  }
  return x;
}

/*!
 *  \brief This function computes the function values associated with
 *  the randomly generated data points
 *  \param x a reference to a vector<double>
 *  \return the list of function values
 */
vector<double> DataGenerator::functionValues(vector<double> &x)
{
  fx = vector<double>(samples,0);
  switch(distribution) {
    case 0: // Normal
    {
      Normal pdf(mu,sigma);
      for (int i=0; i<x.size(); i++) {
        fx[i] = pdf.value(x[i]);
      }
      break;
    }

    case 1: // Laplace
    {
      Laplace pdf(mu,b);
      for (int i=0; i<x.size(); i++) {
        fx[i] = pdf.value(x[i]);
      }
      break;
    }

    default:
      cout << "Distribution not supported ..." << endl;
      exit(1);
  }
  return fx;
}

/*!
 *  \brief This function adds noise to the generated fucntion values
 *  \return the noise added values
 */
vector<double> DataGenerator::addNoise(vector<double> &fx)
{
  Normal pdf(0,noise_sigma);
  vector<double> noise = pdf.generate(samples);
  y = vector<double>(fx);
  for (int i=0; i<samples; i++) {
    y[i] += noise[i];
  }
  return y;
}

/*!
 *  \brief This function plots the random generated data
 */
void DataGenerator::plotData()
{
  pair<double,double> xrange,yrange;

  Plot graph1;
  vector<string> labels1(3,"");
  labels1[0] = "Laplace data plot";
  labels1[1] = "samples";
  labels1[2] = "x";
  graph1.label(labels1);
  xrange.first = 1;
  xrange.second = samples;
  yrange = extremum(x);
  graph1.setRange(xrange,yrange);
  graph1.sketch(x);

  Plot graph2;
  vector<string> labels2(3,"");
  labels2[0] = "Laplace data plot";
  labels2[1] = "x";
  labels2[2] = "f(x)";
  graph2.label(labels2);
  xrange = extremum(x);
  yrange = extremum(fx);
  graph2.setRange(xrange,yrange);
  graph2.sketch(x,fx);

  Plot graph3;
  vector<string> labels3(3,"");
  labels3[0] = "Laplace data plot";
  labels3[1] = "x";
  graph3.label(labels3);
  xrange = extremum(x);
  yrange = extremum(y);
  graph3.setRange(xrange,yrange);
  graph3.sketch(x,fx,y);
}












