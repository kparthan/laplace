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
      Normal pdf(mu,sigma);
      x = pdf.generate(samples);
      break;

    case 1: // Laplace
      Laplace pdf(mu,b);
      x = pdf.generate(samples);
      break;

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
  Plot graph;
  vector<string> labels;
  labels.push_back("Laplace data plot");
  labels.push_back("x");
  labels.push_back("fx");
  graph.label(labels);
  pair<double,double> xrange = extremum(x);
  pair<double,double> fxrange = extremum(fx);
  graph.setRange(xrange,fxrange);
  graph.sketch(x,fx);
}












