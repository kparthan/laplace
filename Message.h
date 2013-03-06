#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"

class Message
{
  private:
    //! Data samples (SORTED) to be encoded
    vector<double> data;

    //! Parameter estimates for the normal distribution
    pair<double,double> normalEstimates;

    //! Parameter estimates for the laplace distribution
    pair<double,double> laplaceEstimates;

  public:
    //! Null constructor
    Message() ;

    //! Constructor
    Message(vector<double> &);

    //! Estimate the parameters for the Normal & Laplace cases
    void estimateParameters();

    //! Estimates the MML parameters for the Normal distribution
    void estimateNormalParameters();

    //! Estimates the MML parameters for the Laplace distribution
    void estimateLaplaceParameters();

    //! Return the parameter estimates for the Normal distribution
    pair<double,double> getNormalEstimates();

    //! Return the parameter estimates for the Laplace distribution
    pair<double,double> getLaplaceEstimates();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Normal distribution
    double encodeUsingNormalModel();

    //! Computes the length of encoding (in bits) for encoding using a
    //! Laplace distribution 
    double encodeUsingLaplaceModel();
};

#endif

