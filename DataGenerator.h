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
      DEFAULT,
      NORMAL,
      LAPLACE
    };

    //! Simulate the experiment
    virtual void simulate(){}

    //! Generate x values
    virtual vector<double> generateRandom(int){}

    //! Calculate the minimum message length and plot
    void estimateAndPlotModel(const char *, vector<double> &, int, int);

    //! Compute the corresponding function values
    virtual vector<double> computeFunctionValues(vector<double> &){}

    //! Add noise to the data
    void addNoise(double);

    //! Predict using the estimates
    vector<vector<double>> predict(vector<double> &, struct MML_Estimates &);

    //! Sort the elements in the list
    vector<double> sort(vector<double> &);

    //! Plot the data
    void plotData(); 

    //! MML estimates
    struct MML_Estimates mmlEstimate(vector<double> &);

  protected:
    //! Distribution type
    DistributionType type;

    //! Model parameters
    struct Parameters parameters;

    //! Random samples generated
    vector<double> x,fx,x_noise,fx_noise;

    //! List of predictions
    vector<vector<double>> predictions, predictions_noise;

    //! Quciksort algorithm
    void quicksort(vector<double> &, vector<int> &, int, int);

    //! Partition function used in quicksort()
    int partition(vector<double> &, vector<int> &, int, int);

    //! Generates the file name to append data to
    string getFileName(const char *, int, int);

    //! Outputs the data to a file
    void writeToFile(string &, int, struct MML_Estimates &);

    //! Outputs the predictions to a file
    void writeToFile(string &, vector<double> &, vector<double> &, 
                     vector<vector<double>> &);

};

#endif

