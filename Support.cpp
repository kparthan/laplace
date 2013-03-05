#include "Support.h"
#include "DataGenerator.h"

/*!
 *  \brief This function checks to see if valid arguments are given to the 
 *  command line output.
 *  \param argc an integer
 *  \param argv an array of strings
 *  \return the parameters to be used in the simulation
 */
struct Parameters parseCommandLineInput(int argc, char **argv)
{
  struct Parameters parameters;
  cout << "Checking command-line input ..." << endl;

  options_description desc("Allowed options");
  desc.add_options()
       ("help","produce help message")
       ("samples",value<int>()->default_value(100),"number of points to be generated")
       ("mu",value<double>()->default_value(0),"mean of the distribution")
       ("scale",value<double>()->default_value(2),"scale parameter")
       ("noise",value<double>()->default_value(0.1),"standard deviation of Gaussian noise")
  ;
  variables_map vm;
  store(parse_command_line(argc,argv,desc),vm);
  notify(vm);

  if (argc == 1 || vm.count("help")) {
    Usage(argv[0],desc);
  }
  
  if (vm.count("samples")) {
    parameters.samples= vm["samples"].as<int>();
    cout << "Number of samples to be generated: " << parameters.samples << endl;
  } 

  if (vm.count("mu")) {
    parameters.mu = vm["mu"].as<double>();
    cout << "Mean of the distribution: " << parameters.mu << endl;
  } 

  if (vm.count("scale")) {
    parameters.scale = vm["scale"].as<double>();
    cout << "Scale parameter: " << parameters.scale << endl;
  }
   
  if (vm.count("noise")) {
    parameters.noise_sigma = vm["noise"].as<double>();
    cout << "Standard deviation of Gaussian noise: " << parameters.noise_sigma << endl;
  }

  return parameters;
}

/*!
 *  \brief This module prints the acceptable input format to the program
 *  \param exe a reference to a const char
 *  \param desc a reference to a options_description object
 */
void Usage (const char *exe, options_description &desc)
{
  cout << "Usage: " << exe << " [options]" << endl;
  cout << desc << endl;
  exit(1);
}

/*!
 *  \brief This module returns the sign of a number.
 *  \param number a double
 *  \return the sign
 */
int sign(double number)
{
  if (fabs(number) <= ZERO) {
    return 0;
  } else if (number > 0) {
    return 1;
  } else {
    return -1;
  }
}

/*!
 *
 */
void laplaceFit(struct Parameters &parameters)
{
  /*cout << "*** Printing parameters ***" << endl;
  cout << "n: " << parameters.samples << endl;
  cout << "mean: " << parameters.mu << endl;
  cout << "scale: " << parameters.b << endl;
  cout << "noise: " << parameters.noise_sigma << endl;*/
  DataGenerator laplace(parameters,0);
  laplace.generateData();
  laplace.plotData();
}

/*!
 *  \brief This module finds the minimum value in a list
 *  \param list a reference to a vector<double>
 *  \return the minimum value in the list
 */
double minimum(vector<double> &list)
{
  double min;
  if (list.size() > 0) {
    min = list[0];
  }
  for (int i=1; i<list.size(); i++) {
    if (list[i] < min) {
      min = list[i];
    }
  }
  return min;
}

/*!
 *  \brief This module finds the maximum value in a list
 *  \param list a reference to a vector<double>
 *  \return the maximum value in the list
 */
double maximum(vector<double> &list)
{
  double max;
  if (list.size() > 0) {
    max = list[0];
  }
  for (int i=1; i<list.size(); i++) {
    if (list[i] > max) {
      max = list[i];
    }
  }
  return max;
}

/*!
 *  \brief This function computes the extremum values in a list
 *  \param list a reference to a vector<double>
 *  \return the minimum and maximum values in the list
 */
pair<double,double> extremum(vector<double> &list)
{
  pair<double,double> ends;
  ends.first = minimum(list);
  ends.second = maximum(list);
  return ends;
}

/*!
 *	\brief This function prints an error message.
 *	\param errorMessage a pointer to a char
 */
void printError(const char* errorMessage)
{
	cerr << "Run-time error ..." << endl ;
	cerr << errorMessage << endl ;
	cerr << "exiting ..." << endl ;
	exit(1) ;
}

