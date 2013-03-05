#ifndef NORMAL_H
#define NORMAL_H

#include "Header.h"

class Normal 
{
  private:
    //! Mean of the distribution
		double mu ;

    //! Standard deviation of the distribution
		double sigma ;

	public:
		//! Constructor
		Normal() ;

		//! Constructor that sets value of parameters
		Normal(double, double);

		//! Function value
		double value(double);

		//! Generates a sample from the distribution
		vector<double> generate(int);

		//! Returns the mean 
		const double mean();

		//! Returns the standard deviation
		const double standardDeviation() ;
};

#endif

