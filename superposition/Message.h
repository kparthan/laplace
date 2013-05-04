#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"

class Message
{
  private:
    //! Data samples (sorted) to be encoded
    vector<vector<double>> samples;

    //! Number of sets of deviations
    int num_deviations_sets;

    //! Estimate method
    int estimate_method;

    //! Parameter estimates for the normal distribution
    vector<array<double,2>> normal_estimates;

    //! Parameter estimates for the laplace distribution
    vector<array<double,2>> laplace_estimates;

    //! Estimates the MML parameters for the Normal distribution
    void estimateNormalParameters();

    //! Estimates the MML parameters for the Laplace distribution
    void estimateLaplaceParameters();

  public:
    //! Null constructor
    Message() ;

    //! Constructor
    Message(vector<array<double,3>> &, int, int);

    //! Return the parameter estimates for the Normal distribution
    vector<array<double,2>> getNormalEstimates();

    //! Return the parameter estimates for the Laplace distribution
    vector<array<double,2>> getLaplaceEstimates();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Normal distribution
    vector<double> encodeUsingNormalModel();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Laplace distribution 
    vector<double> encodeUsingLaplaceModel();
};

#endif

