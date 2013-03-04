#include "Support.h"

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
       ("n",value<int>()->default_value(100),"number of points to be generated")
       ("mu",value<double>()->default_value(0),"mean of the distribution")
       ("b",value<double>()->default_value(2),"scale parameter")
       ("noise",value<double>()->default_value(0.1),"standard deviation of Gaussian noise")
  ;
  variables_map vm;
  store(parse_command_line(argc,argv,desc),vm);
  notify(vm);

  if (argc == 1 || vm.count("help")) {
    Usage(argv[0],desc);
  }
  
  if (vm.count("n")) {
    parameters.samples= vm["n"].as<int>();
    cout << "Number of samples to be generated: " << parameters.samples << endl;
  } 

  if (vm.count("mu")) {
    parameters.mu = vm["mu"].as<double>();
    cout << "Mean of the distribution: " << parameters.mu << endl;
  } 

  if (vm.count("b")) {
    parameters.b = vm["b"].as<double>();
    cout << "Scale parameter: " << parameters.b << endl;
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
 *
 */
void laplaceFit(struct Parameters &parameters)
{
  /*cout << "*** Printing parameters ***" << endl;
  cout << "n: " << parameters.samples << endl;
  cout << "mean: " << parameters.mu << endl;
  cout << "scale: " << parameters.b << endl;
  cout << "noise: " << parameters.noise_sigma << endl;*/
  LaplaceGenerator laplace(parameters);
  vector<double> data = laplace.generate();
}

