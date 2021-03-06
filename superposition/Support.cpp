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
  string encode_deviations;
  string estimate_method;
  
  cout << "Checking command-line input ..." << endl;
  options_description desc("Allowed options");
  desc.add_options()
       ("help","produce help message")
       ("verbose","print some details")
       ("iterate",value<int>(&parameters.iterations),"number of iterations")
       ("accept",value<double>(&parameters.acceptance),"Acceptance probability")
       ("translate",value<double>(&parameters.increment_translation),
                                         "increment during random translation")
       ("rotate",value<double>(&parameters.increment_rotation_angle),
                      "increment in angle (in degrees) during random rotation")
       ("proteins",value<vector<string>>(&parameters.pdb_files)->multitoken(),
                                                             "Input PDB files")
       ("encode",value<string>(&encode_deviations),
                  "method to encodine all the deviations [together/separate]")
       ("estimate",value<string>(&estimate_method),
                        "method used to estimate parameters [custom/general]")
  ;
  variables_map vm;
  store(parse_command_line(argc,argv,desc),vm);
  notify(vm);

  if (vm.count("help")) {
    Usage(argv[0],desc);
  }

  if (vm.count("verbose")) {
    parameters.print = PRINT_DETAIL;
  } else {
    parameters.print = PRINT_NON_DETAIL;
  }

  if (vm.count("proteins")) {
    if (parameters.pdb_files.size() != 2) {
      cout << "Please input two pdb files ..." << endl;
      Usage(argv[0],desc);
    }
    cout << "PDB Files: " << parameters.pdb_files[0] << " "
         << parameters.pdb_files[1] << endl;
  } else {
    cout << "Please input PDB files to superimpose ..." << endl;
    Usage(argv[0],desc);
  }

  if (vm.count("iterate")) {
    cout << "Number of iterations: " << parameters.iterations << endl;
  } else {
    parameters.iterations = DEFAULT_ITERATIONS;
    cout << "Using default number of iterations: " << DEFAULT_ITERATIONS 
         << endl;
  }

  if (vm.count("translate")) {
    cout << "Increment during random translation: " 
         << parameters.increment_translation << endl;
  } else {
    parameters.increment_translation = DEFAULT_INCREMENT_TRANSLATION;
    cout << "Using default value of random increment during translation: "
         << DEFAULT_INCREMENT_TRANSLATION << endl;
  }

  if (vm.count("rotate")) {
    cout << "Increment during random rotation: " 
         << parameters.increment_rotation_angle << " degrees" << endl;
  } else {
    parameters.increment_rotation_angle = DEFAULT_INCREMENT_ROTATION_ANGLE;
    cout << "Using default value of increment of angle of rotation: "
         << DEFAULT_INCREMENT_ROTATION_ANGLE << endl;
  }

  if (vm.count("accept")) {
    cout << "Probability of acceptance: " << parameters.acceptance << endl;
    assert(parameters.acceptance <= 1 && parameters.acceptance >= 0);
  } else {
    parameters.acceptance = DEFAULT_ACCEPTANCE_PROBABILITY;
    cout << "Using default acceptance probability: " 
         << DEFAULT_ACCEPTANCE_PROBABILITY << endl;
  }

  if (vm.count("encode")) {
    if (encode_deviations.compare("together") == 0) {
      parameters.encode_deviations = ENCODE_DEVIATIONS_TOGETHER;
      cout << "Encoding deviations together ..." << endl;
    } else if (encode_deviations.compare("separate") == 0) {
      parameters.encode_deviations = ENCODE_DEVIATIONS_SEPARATE;
      cout << "Encoding deviations separately ..." << endl;
    } else {
      cout << "Incorrect encoding option ..." << endl;
      Usage(argv[0],desc);
    }
  } else {
    parameters.encode_deviations = ENCODE_DEVIATIONS_TOGETHER;
    cout << "Encoding deviations together ..." << endl;
  }

  if (vm.count("estimate")) {
    if (estimate_method.compare("custom") == 0) {
      parameters.estimate_method = ESTIMATE_METHOD_CUSTOM;
      cout << "Estimating one parameter ..." << endl;
    } else if (estimate_method.compare("general") == 0) {
      parameters.estimate_method = ESTIMATE_METHOD_GENERAL;
      cout << "Estimating both parameters ..." << endl;
    } else {
      cout << "Incorrect estimate method option supplied ..." << endl;
      Usage(argv[0],desc);
    }
  } else {
    parameters.estimate_method = ESTIMATE_METHOD_CUSTOM;
    cout << "Estimating one parameter ..." << endl;
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
 *  \brief This module checks whether the input file exists or not.
 *  \param fileName a character string
 *  \return true or false depending on whether the file exists or not.
 */
bool checkFile(const char *fileName)
{
  ifstream file(fileName);
  return file;
}

/*!
 *  \brief This module parses the input PDB file.
 *  \param pdbFile a pointer to a character array
 *  \return a pointer to a ProteinStructure object
 */
ProteinStructure *parsePDBFile(const char *pdbFile)
{
  if(!checkFile(pdbFile)){
    cout << "\nFile \"" << pdbFile << "\" does not exist ..." << endl;
    exit(1);
  }
  cout << "Parsing PDB file ...";
  BrookhavenPDBParser parser;
  ProteinStructure *structure = parser.getStructure(pdbFile)->select(CASelector());
  cout << " [OK]" << endl;
  return structure;
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

