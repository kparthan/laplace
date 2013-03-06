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
 */
Message::Message(vector<double> &data) : data(data)
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
  double mean = computeMean(data); 
  normalEstimates.first = mean;
  double sum = 0;
  for (int i=0; i<data.size(); i++) {
    sum += (data[i] - mean) * (data[i] - mean);
  }
  double sigma_square = sum / (double)(data.size()-1);
  normalEstimates.second = sqrt(sigma_square);
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Laplace distribution
 */
void Message::estimateLaplaceParameters()
{
  double median = computeMedian(data);
  laplaceEstimates.first = median;
  double sum = 0;
  for (int i=0; i<data.size(); i++) {
    sum += fabs(data[i]-median);
  }
  laplaceEstimates.second = sum / (double)(data.size()-1);
}

/*!
 *  \brief This function returns the Normal estimates of the parameters
 *  \return the Normal estimates
 */
pair<double,double> Message::getNormalEstimates()
{
  return normalEstimates;
}

/*!
 *  \brief This function returns the Laplace estimates of the parameters
 *  \return the Laplace estimates
 */
pair<double,double> Message::getLaplaceEstimates()
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
  double rangeLogSigma = 3.0;
  int N = data.size();
  double part1 = log(K2) + log(rangeMu * rangeLogSigma) + 0.5 * log(2 * N * N) 
                 - log(normalEstimates.second);
  double part2 = (N/2.0) * log(2 * PI) - N * log(AOM) + 0.5 * (N+1) + 
                  + N * log(normalEstimates.second);
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
  double rangeSigma = 3.0;
  int N = data.size();
  double part1 = log(K2) + log(rangeMu * rangeSigma) + log(N)
                 - log(laplaceEstimates.second);
  double part2 = N * log(2/AOM) + N * log(laplaceEstimates.second) + N;
  return (part1+part2)/log(2);
}

