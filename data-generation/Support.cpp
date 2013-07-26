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

    /*if (vm.count("estimate")) {
      cout << "Distribution which is used to estimate the data: " 
           << parameters.estimate << endl;
    } else {
      parameters.estimate = DEFAULT_ESTIMATE_DISTRIBUTION;
      cout << "Data estimated using the default distribution: "
           << DEFAULT_ESTIMATE_DISTRIBUTION << endl;
    }*/

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
    if (parameters.iterations > 1) {
      analyzeScale(parameters);
      analyzeScaleVariance(parameters);
      analyzeDiffMsglen(parameters);
    }
  } else if (parameters.generate == UNSET) {  // data file provided
    vector<double> x = parseFile(parameters.data_file);
    DataGenerator data;
    vector<double> sorted = sort(x);
    vector<double> y = data.modifyDataToAOM(sorted,parameters.aom);
    /*if (parameters.print == SET) {
      for (int i=0; i<y.size(); i++) {
        cout << y[i] << " ";
      }
      cout << endl;
    }*/
    struct Estimates estimates = data.parameterEstimation(y,parameters.aom);
    printEstimates(estimates);
  }
}

/*!
 *  \brief This function prints the parameter estimates of the data
 *  \param estimates a reference to a struct Estimates
 */
void printEstimates(struct Estimates &estimates)
{
  cout << "\n*** NORMAL ESTIMATES ***" << endl;
  cout << "normal_mean: " << estimates.normal_mean << endl;
  cout << "normal_scale_ml: " << estimates.normal_scale_ml << endl;
  cout << "normal_scale_mml: " << estimates.normal_scale_mml << endl;
  cout << "normal_msglen: " << estimates.normal_msglen << endl;
  cout << "\n*** LAPLACE ESTIMATES ***" << endl;
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

void analyzeScale(struct Parameters &parameters)
{
  int col1,col2;
  if (boost::iequals(parameters.distribution,"laplace")) {
    col1 = 7; col2 = 8;
  } else if (boost::iequals(parameters.distribution,"normal")) {
    col1 = 2; col2 = 3;
  }
  double b;
  int n;
  for (int i=0; i<parameters.scale.size(); i++) {
    b = parameters.scale[i];

    // read the estimated scale values into a matrix
    vector<vector<double>> bml,bmml; // N X num_iterations
    vector<vector<double>> bml_range,bmml_range;
    for (int j=0; j<parameters.samples.size(); j++) {
      n = parameters.samples[j];
      string file_name = fileName(parameters.distribution,parameters.mean,
                                  n,b,parameters.iterations);
      vector<double> bmln,bmmln;
      vector<double> bmln_range,bmmln_range;
      bmln = getColumn(file_name,col1);
      bmln_range = getRange(bmln);
      bml.push_back(bmln);
      bmmln = getColumn(file_name,col2);
      bmmln_range = getRange(bmmln);
      bmml.push_back(bmmln);
    }

    // print the matrix values to a file
    string file_ml = "results/data/" + parameters.distribution + "_ml_scale_" + 
                     boost::lexical_cast<string>(b).substr(0,3) + "_iter_" +
                     boost::lexical_cast<string>(parameters.iterations);
    string file_mml = "results/data/" + parameters.distribution + "_mml_scale_" + 
                     boost::lexical_cast<string>(b).substr(0,3) + "_iter_" +
                     boost::lexical_cast<string>(parameters.iterations);
    ofstream file1(file_ml.c_str());
    ofstream file2(file_mml.c_str());
    for (int j=0; j<parameters.iterations; j++) {
      for (int k=0; k<parameters.samples.size(); k++) {
        file1 << fixed << setw(10) << setprecision(4) << bml[k][j];
        file2 << fixed << setw(10) << setprecision(4) << bmml[k][j];
      }
      file1 << endl;
      file2 << endl;
    }

    // for each scale parameter
    plotScaleBoxplots(n,b,file_ml,file_mml,parameters);
  }
}

void plotScaleBoxplots(int n, double b, string file_ml, string file_mml, 
                       struct Parameters &parameters)
{
  string eps_file_ml =  "results/plots/boxplot_" + parameters.distribution + "_ml_scale_" + 
                     boost::lexical_cast<string>(b).substr(0,3) + "_iter_" +
                     boost::lexical_cast<string>(parameters.iterations) + ".eps";
  string eps_file_mml =  "results/plots/boxplot_" + parameters.distribution + "_mml_scale_" + 
                     boost::lexical_cast<string>(b).substr(0,3) + "_iter_" +
                     boost::lexical_cast<string>(parameters.iterations) + ".eps";

  ofstream script("results/script.plot");
  script << "set terminal postscript eps enhanced" << endl;
  script << "#set linetype 1 lc rgb \"blue\"" << endl;
  script << "#set style fill solid 0.25 border lt 1" << endl;
  script << "set style boxplot outliers pointtype 7" << endl;
  script << "set style data boxplot" << endl;
  script << "set boxwidth  0.05" << endl;
  script << "set pointsize 0.5" << endl;
  script << "unset key" << endl;
  script << "set border 2" << endl;
  script << "set xtics nomirror" << endl;
  script << "set ytics nomirror" << endl;
  script << "set xlabel \"Number of samples\"" << endl;
  script << "set ylabel \"Scale parameter\"" << endl;
  script << "#set grid ytics" << endl;
  script << "set xrange [0:*]" << endl;
  script << "set xtics (";
  for (int i=0; i<parameters.samples.size(); i++) {
    if (i != parameters.samples.size() - 1) {
      script << "\"" << parameters.samples[i] << "\" " << i+1 << ", ";
    } else {
      script << "\"" << parameters.samples[i] << "\" " << i+1 << ") scale 0.0" << endl;
    }
  }
  script << "set output \"" << eps_file_ml << "\"" << endl;
  script << "plot ";
  for (int i=0; i<parameters.samples.size(); i++) {
    if (i != parameters.samples.size() - 1) {
      script << "\"" << file_ml << "\" using (" << i+1 << "):" << i+1 
             << "lt 1 lc rgb \"red\" pointtype 7" << ", "; 
    } else {
      script << "\"" << file_ml << "\" using (" << i+1 << "):" << i+1 
             << "lt 1 lc rgb \"red\" pointtype 7" << ", " << b 
             << "with lines lt 4" << endl; 
    }
  }  
  script << "set output \"" << eps_file_mml << "\"" << endl;
  script << "plot ";
  for (int i=0; i<parameters.samples.size(); i++) {
    if (i != parameters.samples.size() - 1) {
      script << "\"" << file_mml << "\" using (" << i+1 << "):" << i+1 
             << "lt 1 lc rgb \"blue\" pointtype 7" << ", "; 
    } else {
      script << "\"" << file_mml << "\" using (" << i+1 << "):" << i+1 
             << "lt 1 lc rgb \"blue\" pointtype 7" << ", " << b 
             << "with lines lt 4" << endl; 
    }
  }  
  script.close();
  system("gnuplot -persist results/script.plot");
}

void analyzeScaleVariance(struct Parameters &parameters)
{
}

void analyzeDiffMsglen(struct Parameters &parameters) {
}

vector<double> getColumn(string file_name, int index)
{
  string line;
  vector<double> numbers;
  ifstream file(file_name.c_str());
  while(getline(file,line)) {
    boost::char_separator<char> sep(", ");
    boost::tokenizer<boost::char_separator<char> > tokens(line,sep);
    int count = 1;
    BOOST_FOREACH (const string& t, tokens) {
      istringstream iss(t);
      if (count == index) {
        double x;
        iss >> x;
        numbers.push_back(x);
      }
      count++;
    }
  }
  file.close();
  return numbers;
}

vector<double> getRange(vector<double> &list)
{
  vector<double> range(3,0);
  //for (int i)
  return range;
}

string fileName(string distribution, double mean, int n, double b, 
                int iterations)
{
  string file = "results/data/statistics_" + distribution;
  file += "_n_" + boost::lexical_cast<string>(n);
  file += "_mean_" + boost::lexical_cast<string>(mean);
  file += "_scale_" + boost::lexical_cast<string>(b).substr(0,3);
  file += "_iter_" + boost::lexical_cast<string>(iterations);
  //cout << file << endl;
  return file;
}

/*!
 *  \brief This function sorts the elements in the list
 *  \param list a reference to a vector<double>
 *  \return the sorted list
 */
vector<double> sort(vector<double> &list)
{
  int num_samples = list.size();
	vector<double> sortedList(list);
  vector<int> index(num_samples,0);
	for(int i=0; i<num_samples; i++) {
			index[i] = i;
  }
	quicksort(sortedList,index,0,num_samples-1);
  return sortedList;
}

/*!
 *  This is an implementation of the classic quicksort() algorithm to sort a
 *  list of data values. The module uses the overloading operator(<) to 
 *  compare two Point<T> objects. 
 *  Pivot is chosen as the right most element in the list(default)
 *  This function is called recursively.
 *  \param list a reference to a vector<double>
 *	\param index a reference to a vector<int>
 *  \param left an integer
 *  \param right an integer
 */
void quicksort(vector<double> &list, vector<int> &index, 
                              int left, int right)
{
	if(left < right)
	{
		int pivotNewIndex = partition(list,index,left,right);
		quicksort(list,index,left,pivotNewIndex-1);
		quicksort(list,index,pivotNewIndex+1,right);
	}
}

/*!
 *  This function is called from the quicksort() routine to compute the new
 *  pivot index.
 *  \param list a reference to a vector<double>
 *	\param index a reference to a vector<int>
 *  \param left an integer
 *  \param right an integer
 *  \return the new pivot index
 */
int partition(vector<double> &list, vector<int> &index,
                             int left, int right)
{
	double temp,pivotPoint = list[right];
	int storeIndex = left,temp_i;
	for(int i=left; i<right; i++) {
		if(list[i] < pivotPoint) {
			temp = list[i];
			list[i] = list[storeIndex];
			list[storeIndex] = temp;
			temp_i = index[i];
			index[i] = index[storeIndex];
			index[storeIndex] = temp_i;
			storeIndex += 1;	
		}
	}
	temp = list[storeIndex];
	list[storeIndex] = list[right];
	list[right] = temp;
	temp_i = index[storeIndex];
	index[storeIndex] = index[right];
	index[right] = temp_i;
	return storeIndex;
}

