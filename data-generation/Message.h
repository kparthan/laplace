#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"

class Message
{
  private:
    //! Data samples (sorted) to be encoded
    vector<double> data;

    //! Parameter estimates for the normal distribution
    vector<double> normalEstimates;

    //! Parameter estimates for the laplace distribution
    vector<double> laplaceEstimates;

    //! Estimates the MML parameters for the Normal distribution
    void estimateNormalParameters();

    //! Estimates the MML parameters for the Laplace distribution
    void estimateLaplaceParameters();

  public:
    //! Null constructor
    Message() ;

    //! Constructor
    Message(vector<double> &);

    //! Estimate the parameters for the Normal & Laplace cases
    void estimateParameters();

    //! Return the parameter estimates for the Normal distribution
    vector<double> getNormalEstimates();

    //! Return the parameter estimates for the Laplace distribution
    vector<double> getLaplaceEstimates();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Normal distribution
    double encodeUsingNormalModel();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Laplace distribution 
    double encodeUsingLaplaceModel();
};

#endif

