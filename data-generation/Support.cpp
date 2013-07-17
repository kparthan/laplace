#include "Support.h"
#include "NormalDataGenerator.h"
#include "LaplaceDataGenerator.h"

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
       ("verbose","print detailed output")
       ("samples",value<vector<int>>(&parameters.samples)->multitoken(),
                                    "number of points to be generated")
       ("mean",value<double>(&parameters.mean),"mean of the distribution")
       ("scale",value<vector<double>>(&parameters.scale)->multitoken(),
                                                        "the scale values")
       ("generate",value<string>(&parameters.distribution),
                                    "distribution used to generate the data")
       ("estimate",value<string>(&parameters.estimate),
                                    "distribution used to estimate the data")
       ("iterate",value<int>(&parameters.iterations),"number of iterations")
       ("data",value<string>(&parameters.data_file),"data file")
       ("aom",value<double>(&parameters.aom),"AOM of data")
  ;
  variables_map vm;
  store(parse_command_line(argc,argv,desc),vm);
  notify(vm);

  if (argc == 1 || vm.count("help")) {
    Usage(argv[0],desc);
  }

  if (vm.count("data")) {
    parameters.generate = UNSET;
  } else {
    parameters.generate = SET;
  }

  if (!vm.count("aom")) {
    parameters.aom = DEFAULT_AOM;
    cout << "Using default AOM: " << parameters.aom << endl;
  } else {
    cout << "Using AOM: " << parameters.aom << endl;
  }
   
  if (vm.count("verbose")) {
    parameters.print = SET;
  } else {
    parameters.print = UNSET;
  }
  
  if (parameters.generate == SET) {
    if (vm.count("samples")) {
      cout << "Number of samples to be generated: ";
      for (int i=0; i<parameters.samples.size(); i++) {
        cout << parameters.samples[i] << " ";
      }
      cout << endl;
    } else {
      parameters.samples = vector<int>(1,DEFAULT_SAMPLES);
      cout << "Generating default number of samples: " << DEFAULT_SAMPLES << endl;
    }

    if (vm.count("mean")) {
      cout << "Mean of the distribution: " << parameters.mean << endl;
    } else {
      parameters.mean = DEFAULT_MEAN;
      cout << "Using default mean: " << DEFAULT_MEAN << endl;
    }

    if (vm.count("scale")) {
      cout << "Values of the scale parameter: ";
      for (int i=0; i<parameters.scale.size(); i++) {
        cout << parameters.scale[i] << " ";
      }
      cout << endl;
    } else {
      parameters.scale = vector<double>(1,DEFAULT_SCALE);
      cout << "Using default scale parameter: " << DEFAULT_SCALE << endl;
    }

    if (vm.count("iterate")) {
      cout << "Number of iterations: " << parameters.iterations << endl;
    } else {
      parameters.iterations = DEFAULT_ITERATIONS;
      cout << "Default number of iterations: " << DEFAULT_ITERATIONS << endl;
    }

    if (vm.count("generate")) {
      cout << "Distribution from which data is generated: " 
           << parameters.distribution << endl;
    } else {
      parameters.distribution = DEFAULT_GENERATE_DISTRIBUTION;
      cout << "Data generated using the default distribution: "
           << DEFAULT_GENERATE_DISTRIBUTION << endl;
    }

    if (vm.count("estimate")) {
      cout << "Distribution which is used to estimate the data: " 
           << parameters.estimate << endl;
    } else {
      parameters.estimate = DEFAULT_ESTIMATE_DISTRIBUTION;
      cout << "Data estimated using the default distribution: "
           << DEFAULT_ESTIMATE_DISTRIBUTION << endl;
    }

    cout << "Samples: ";
    for (int i=0; i<parameters.samples.size(); i++) {
      cout << parameters.samples[i] << " ";
    }
    cout << endl;
    cout << "Mean: " << parameters.mean << endl;
    cout << "Scale: ";
    for (int i=0; i<parameters.scale.size(); i++) {
      cout << parameters.scale[i] << " ";
    }
    cout << endl;
    cout << "Distribution: " << parameters.distribution << endl;
    //cout << "Estimate: " << parameters.estimate << endl;
    return parameters;
  }
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
 *  \brief This functions fit the data to a distribution
 *  \param parameters a reference to a struct Parameters
 */
void fitData(struct Parameters &parameters)
{
  if (parameters.generate == SET) { // data file not provided
    if (boost::iequals(parameters.distribution,"laplace")) {
      LaplaceDataGenerator laplace_data_generator(parameters);
      DataGenerator *data_generator = &laplace_data_generator;
      data_generator->simulate();
    } else if (boost::iequals(parameters.distribution,"normal")) {
      NormalDataGenerator normal_data_generator(parameters);
      DataGenerator *data_generator = &normal_data_generator;
      data_generator->simulate();
    } else {
      cout << "Distribution unknown ..." << endl;
      exit(1);
    }
  } else if (parameters.generate == UNSET) {  // data file provided
    vector<double> x = parseFile(parameters.data_file);
    DataGenerator data;
    vector<double> sorted = data.sort(x);
    vector<double> y = data.modifyDataToAOM(sorted,parameters.aom);
    if (parameters.print == SET) {
      for (int i=0; i<y.size(); i++) {
        cout << y[i] << " ";
      }
      cout << endl;
    }
    struct Estimates estimates = data.mmlEstimate(y,parameters.aom);
    printEstimates(estimates);
  }
}

/*!
 *  \brief This function prints the parameter estimates of the data
 *  \param estimates a reference to a struct Estimates
 */
void printEstimates(struct Estimates &estimates)
{
  cout << "normal_mean: " << estimates.normal_mean << endl;
  cout << "normal_sigma_ml: " << estimates.normal_sigma_ml << endl;
  cout << "normal_sigma_mml: " << estimates.normal_sigma_mml << endl;
  cout << "normal_msglen: " << estimates.normal_msglen << endl;
  cout << "laplace_mean: " << estimates.laplace_mean << endl;
  cout << "laplace_scale_ml: " << estimates.laplace_scale_ml << endl;
  cout << "laplace_scale_mml: " << estimates.laplace_scale_mml << endl;
  cout << "laplace_msglen: " << estimates.laplace_msglen << endl;
}

/*!
 *  \brief This method reads the data of a file
 *  \param file_name a reference to a string
 *  \return the list of data values
 */
vector<double> parseFile(string &file_name)
{
  ifstream file(file_name.c_str());
  string line;
  vector<double> numbers;
  while(getline(file,line)) {
    boost::char_separator<char> sep(", ");
    boost::tokenizer<boost::char_separator<char> > tokens(line,sep);
    BOOST_FOREACH (const string& t, tokens) {
      istringstream iss(t);
      double x;
      iss >> x;
      numbers.push_back(x);
    }
  }
  cout << "Input data size: " << numbers.size() << endl;
  return numbers;
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

/*!
 *  \brief This module computes the mean of a set of samples
 *  \param list a reference to a vector<double>
 *  \return the mean value
 */
double computeMean(vector<double> &list)
{
  double sum = 0;
  for (int i=0; i<list.size(); i++) {
    sum += list[i];
  }
  return sum / (double)list.size();
}

/*!
 *  \brief This module computes the median of a sorted set of samples
 *  \param list a reference to a vector<double>
 *  \return the median value
 */
double computeMedian(vector<double> &list)
{
  int n = list.size();
  if (n % 2 == 1) {
    return list[n/2];
  } else {
    return (list[n/2-1]+list[n/2])/2;
  }
}

/*!
 *  \brief This function is used to convert a double to a string
 *  \param number a double
 *  \return a string
 */
string convertToString(double number)
{
  ostringstream convert ;
  convert << number ;
  return convert.str() ;
}

