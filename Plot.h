#ifndef PLOT_H
#define PLOT_H

#include "Support.h"

class Plot
{
	private:
    //! Data file
    string script_file;

		//! Gnuplot script file
		ofstream script;

    //! Print default specifications
    void specifyDefault();

	public:
		//! Constructor
		Plot();

		//! Labeling the axes & naming the graph
		void label(vector<string> &);

		//! Labeling the axes & naming the graph
		void label(string, vector<string> &);

		//! setting the ranges of X and Y axes
		void setRange(pair<double,double>, pair<double,double>);

		//! plotting the data values(X's only)
		void sketch(vector<double> &);

		//! plotting the data values(X vs fx)
		void sketch(vector<double> &, vector<double> &);

		//! plotting the data values(X vs fx vs y)
		void sketch(vector<double> &, vector<double> &, vector<double> &);
};

#endif

