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
       ("verbose","print some details")
       ("iterate",value<int>(&parameters.iterations),"number of iterations")
       ("accept",value<double>(&parameters.acceptance),"Acceptance probability")
       ("move",value<double>(&parameters.move),
                                         "increment during random translation")
       ("proteins",value<vector<string>>(&parameters.pdb_files)->multitoken(),
                                                             "Input PDB files")
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

  if (vm.count("move")) {
    cout << "Random increment during translation: " << parameters.move << endl;
  } else {
    parameters.move = DEFAULT_RANDOM_MOVE;
    cout << "Using default value of random increment: " 
         << DEFAULT_RANDOM_MOVE << endl;
  }

  if (vm.count("accept")) {
    cout << "Probability of acceptance: " << parameters.acceptance << endl;
    assert(parameters.acceptance <= 1 && parameters.acceptance >= 0);
  } else {
    parameters.acceptance = DEFAULT_ACCEPTANCE_PROBABILITY;
    cout << "Using default acceptance probability: " 
         << DEFAULT_ACCEPTANCE_PROBABILITY << endl;
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

