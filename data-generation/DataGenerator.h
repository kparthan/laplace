#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "Support.h"

class DataGenerator
{
  public:
    //! Null constructor
    DataGenerator();

    //! Constructor
    DataGenerator(struct Parameters &);

    //! Types of distributions
    enum DistributionType {
      NORMAL = 0,
      LAPLACE = 1,
      DEFAULT = 2
    };

    //! Simulate the experiment
    virtual void simulate(){}

    //! Generate x values
    virtual vector<double> generateRandom(int){}

    //! Modify data samples to required precision
    vector<double> modifyDataToAOM(vector<double> &, double);

    //! Calculate the minimum message length and plot
    Estimates estimateAndPlotModel(const char *, vector<double> &, int);

    //! Plot the message lengths for all iterations
    void plotMessageLength(const char *, int, int);

    //! Plots the errors in parameter estimates
    void plotErrors(const char *);

    //! Compute the corresponding function values
    virtual vector<double> computeFunctionValues(vector<double> &){}

    //! Predict using the estimates
    vector<vector<double>> predict(vector<double> &, struct Estimates &);

    //! Sort the elements in the list
    vector<double> sort(vector<double> &);

    //! Plot the data
    void plotPredictions(string &); 

    //! MML estimates
    struct Estimates mmlEstimate(vector<double> &, double);

  protected:
    //! Distribution type
    DistributionType type;

    //! Model parameters
    struct Parameters parameters;

    //! Error statistics
    struct Statistics statistics;

    //! Random samples generated
    vector<double> x,fx;

    //! List of predictions
    vector<vector<double>> predictions;

    //! Quciksort algorithm
    void quicksort(vector<double> &, vector<int> &, int, int);

    //! Partition function used in quicksort()
    int partition(vector<double> &, vector<int> &, int, int);

    //! Generates the file name to append data to
    string getFileName(const char *, int, int);

    //! Outputs the data to a file
    void updateResults(string &, int, int, struct Estimates &);

    //! Updates the statistics over iterations
    void updateStatistics(int, int, struct Estimates &, struct Statistics &);

    //! Saves the statistics to a file
    void saveErrorStatistics(const char *, struct Statistics &, int, int, int);

    //! Outputs the predictions to a file
    void writeToFile(string &, vector<double> &, vector<double> &, 
                     vector<vector<double>> &);

};

#endif

