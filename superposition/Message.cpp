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
Message::Message(vector<array<double,3>> &deviations, int num_deviations_sets,
                 int estimate_method) : estimate_method(estimate_method),
                 num_deviations_sets(num_deviations_sets)
{
  vector<double> tmp;
  if (num_deviations_sets == 3) {
    for (int i=0; i<num_deviations_sets; i++){
      for (int j=0; j<deviations.size(); j++){
        tmp.push_back(deviations[j][i]);
      }
      samples.push_back(tmp);
      cout << samples[i].size() << endl;
      tmp.clear();
    }
  } else if (num_deviations_sets == 1) {
    for (int i=0; i<deviations.size(); i++) {
      for (int j=0; j<3; j++) {
        tmp.push_back(deviations[i][j]);
      }
    }
    samples.push_back(tmp);
    cout << samples[0].size() << endl;
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
    double mean;
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      mean = 0;
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      mean = computeMean(samples[i]);
    }
    estimates[0] = mean;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += (samples[i][j] - mean) * (samples[i][j] - mean);
    }
    double sigma_square;
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      sigma_square = sum / (double)samples[i].size();
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      sigma_square = sum / (double)(samples[i].size()-1);
    }
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
    double median;
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      median = 0;
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      median = computeMedian(samples[i]);
    }
    estimates[0] = median;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += fabs(samples[i][j]-median);
    }
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      estimates[1] = sum / (double)samples[i].size();
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      estimates[1] = sum / (double)(samples[i].size()-1);
    }
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
  double K1 = 1.0 / 12;
  double K2 = 5.0 / (36 * sqrt(3));
  double range_mu = 5.0;
  double range_log_sigma = log(4.0/(3*AOM));
  //double range_log_sigma = log(2.0);
  for (int i=0; i<num_deviations_sets; i++) {
    int N = samples[i].size();
    double part1,part2;
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      part1 = 0.5 * log(K1) + 0.5 * log(2 * N) + log(range_log_sigma);
      part2 = 0.5 * (N+1) + (N/2.0)*log(2 * PI) - N * log(AOM)
                 + N * log(normal_estimates[i][1]);
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      part1 = log(K2) + log(range_mu * range_log_sigma) + 0.5 * log(2 * N * N) 
                   - log(normal_estimates[i][1]);
      part2 = (N/2.0) * log(2 * PI) - N * log(AOM) + 0.5 * (N+1) + 
                    + N * log(normal_estimates[i][1]);
    }
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
  double K1 = 1.0 / 12;
  double K2 = 5.0 / (36 * sqrt(3));
  double range_mu = 5.0;
  double range_log_scale = log(4.0/(3*AOM));
  //double range_log_scale = log(2.0);
  for (int i=0; i<num_deviations_sets; i++) {
    int N = samples[i].size();
    double part1,part2;
    if (estimate_method == ESTIMATE_METHOD_CUSTOM) {
      part1 = 0.5 * log(K1) + 0.5 * log(N) + log(range_log_scale);
      part2 = N * log(2/AOM) + N * log(laplace_estimates[i][1])
              + N + 0.5;
    } else if (estimate_method == ESTIMATE_METHOD_GENERAL) {
      part1 = log(K2) + log(range_mu * range_log_scale) + log(N)
                   - log(laplace_estimates[i][1]);
      part2 = N * log(2/AOM) + N * log(laplace_estimates[i][1]) + N;
    }
    msglen[i] = (part1+part2)/log(2);
  }
  return msglen;
}

