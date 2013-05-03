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
 *  \param num_deviations_sets an integer
 */
Message::Message(vector<array<double,3>> &deviations, int num_deviations_sets):
                 num_deviations_sets(num_deviations_sets)
{
  vector<double> tmp;
  for (int i=0; i<num_deviations_sets; i++){
    for (int j=0; j<deviations.size(); j++){
      tmp.push_back(deviations[j][i]);
    }
    samples.push_back(tmp);
    tmp.clear();
  }
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Normal distribution
 */
void Message::estimateNormalParameters()
{
  for (int i=0; i<num_deviations_sets; i++) {
    array<double,2> estimates;
    double mean = computeMean(samples[i]); 
    //double mean = 0;
    estimates[0] = mean;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += (samples[i][j] - mean) * (samples[i][j] - mean);
    }
    double sigma_square = sum / (double)(samples[i].size()-1);
    estimates[1] = sqrt(sigma_square);
    normal_estimates.push_back(estimates);
  }
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Laplace distribution
 */
void Message::estimateLaplaceParameters()
{
  for (int i=0; i<num_deviations_sets; i++) {
    array<double,2> estimates;
    vector<double> list = sort(samples[i]);
    double median = computeMedian(list);
    //double median = 0;
    estimates[0] = median;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += fabs(samples[i][j]-median);
    }
    estimates[1] = sum / (double)(samples[i].size()-1);
    laplace_estimates.push_back(estimates);
  }
}

/*!
 *  \brief This function returns the Normal estimates of the parameters
 *  \return the Normal estimates
 */
vector<array<double,2>> Message::getNormalEstimates()
{
  estimateNormalParameters();
  return normal_estimates;
}

/*!
 *  \brief This function returns the Laplace estimates of the parameters
 *  \return the Laplace estimates
 */
vector<array<double,2>> Message::getLaplaceEstimates()
{
  estimateLaplaceParameters();
  return laplace_estimates;
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
vector<double> Message::encodeUsingNormalModel()
{
  vector<double> msglen(num_deviations_sets,0);
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 2.0;
  double rangeLogSigma = log(2.0);
  for (int i=0; i<num_deviations_sets; i++) {
    int N = samples[i].size();
    double part1 = log(K2) + log(rangeMu * rangeLogSigma) + 0.5 * log(2 * N * N) 
                   - log(normal_estimates[i][1]);
    double part2 = (N/2.0) * log(2 * PI) - N * log(AOM) + 0.5 * (N+1) + 
                    + N * log(normal_estimates[i][1]);
    msglen[i] = (part1+part2)/log(2);
  }
  return msglen;
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
vector<double> Message::encodeUsingLaplaceModel()
{
  vector<double> msglen(num_deviations_sets,0);
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 2.0;
  double rangeSigma = log(2.0);
  for (int i=0; i<num_deviations_sets; i++) {
    int N = samples[i].size();
    double part1 = log(K2) + log(rangeMu * rangeSigma) + log(N)
                   - log(laplace_estimates[i][1]);
    double part2 = N * log(2/AOM) + N * log(laplace_estimates[i][1]) + N;
    msglen[i] = (part1+part2)/log(2);
  }
  return msglen;
}

