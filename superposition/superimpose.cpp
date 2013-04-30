#include "Support.h"
#include "Simulation.h"

int main(int argc, char **argv)
{
  struct Parameters parameters = parseCommandLineInput(argc,argv);

  clock_t c_start = clock();
  auto t_start = high_resolution_clock::now();

  ProteinStructure *moving = parsePDBFile(parameters.pdb_files[0].c_str());
  ProteinStructure *fixed = parsePDBFile(parameters.pdb_files[1].c_str());
  Simulation simulation(moving,fixed,parameters);
  simulation.initialSuperposition();
  ProteinStructure final = simulation.perturb();
  simulation.computeMessageLength();
  simulation.computeMessageLength(final);

  clock_t c_end = clock();
  auto t_end = high_resolution_clock::now();

  cout << endl;
  cout << "CPU time used: " 
  << double(c_end-c_start)/(double)(CLOCKS_PER_SEC) << " secs." << endl;
  cout << "Wall clock time elapsed: "
  << duration_cast<seconds>(t_end-t_start).count() << " secs." << endl;
  return 0;
}

