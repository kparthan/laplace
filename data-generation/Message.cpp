#include "Support.h"
#include "Message.h"

/*!
 *  \brief This is a null constructor used to instantiate the object
 */
Message::Message()
{}

/*!
 *  \brief This is a constructor function used to instantiate the object
 *  \param deviations a reference to a vector<array<double,3>>
 *  \param aom a double
 */
Message::Message(vector<double> &data, double aom) : data(data), AOM(aom)
{}

/*!
 *  \brief This function estimates the parameters for the Normal and
 *  Laplace distributions
 */
void Message::estimateParameters()
{
  estimateNormalParameters();
  estimateLaplaceParameters();
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Normal distribution
 */
void Message::estimateNormalParameters()
{
  normalEstimates = vector<double>(3,0);
  double mean = computeMean(data); 
  normalEstimates[0] = mean;
  double sum = 0;
  for (int i=0; i<data.size(); i++) {
    sum += (data[i] - mean) * (data[i] - mean);
  }
  double sigma_square = sum / (double)(data.size());
  normalEstimates[1] = sqrt(sigma_square);  // ML estimate of sigma
  sigma_square = sum / (double)(data.size()-1);
  normalEstimates[2] = sqrt(sigma_square);  // MML estimate of sigma
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Laplace distribution
 */
void Message::estimateLaplaceParameters()
{
  laplaceEstimates = vector<double>(3,0);
  double median = computeMedian(data);
  laplaceEstimates[0] = median;
  double sum = 0;
  for (int i=0; i<data.size(); i++) {
    sum += fabs(data[i]-median);
  }
  laplaceEstimates[1] = sum / (double)(data.size());  // ML estimate of b
  laplaceEstimates[2] = sum / (double)(data.size()-1); // MML estimate of b
}

/*!
 *  \brief This function returns the Normal estimates of the parameters
 *  \return the Normal estimates
 */
vector<double> Message::getNormalEstimates()
{
  return normalEstimates;
}

/*!
 *  \brief This function returns the Laplace estimates of the parameters
 *  \return the Laplace estimates
 */
vector<double> Message::getLaplaceEstimates()
{
  return laplaceEstimates;
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
double Message::encodeUsingNormalModel()
{
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 5.0;
  double rangeLogSigma = log(4/(3*AOM));
  int N = data.size();
  double part1 = log(K2) + log(rangeMu * rangeLogSigma) + 0.5 * log(2 * N * N) 
                 - log(normalEstimates[2]);
  double part2 = (N/2.0) * log(2 * PI) - N * log(AOM) + 0.5 * (N+1) + 
                  + N * log(normalEstimates[2]);
  return (part1+part2)/log(2);
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
double Message::encodeUsingLaplaceModel()
{
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 5.0;
  double rangeLogSigma = log(4/(3*AOM));
  int N = data.size();
  double part1 = log(K2) + log(rangeMu * rangeLogSigma) + log(N)
                 - log(laplaceEstimates[2]);
  double part2 = N * log(2/AOM) + N * log(laplaceEstimates[2]) + N;
  return (part1+part2)/log(2);
}

