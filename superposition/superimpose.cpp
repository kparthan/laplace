#include "Support.h"
#include "Simulation.h"

int main(int argc, char **argv)
{
  struct Parameters parameters = parseCommandLineInput(argc,argv);

  clock_t c_start = clock();
  auto t_start = high_resolution_clock::now();

  ofstream results("results_general_2",ios::app);
  results << parameters.pdb_files[0] << " " << parameters.pdb_files[1] << "\t"; 
  results.close();

  ProteinStructure *moving = parsePDBFile(parameters.pdb_files[0].c_str());
  ProteinStructure *fixed = parsePDBFile(parameters.pdb_files[1].c_str());
  Simulation simulation(moving,fixed,parameters);
  simulation.initialSuperposition();
  ProteinStructure protein = *moving;
  ProteinStructure final = simulation.perturb();

  double m1,m2,m3;
  m1 = simulation.computeMessageLength();
  m2 = simulation.computeMessageLength(protein);
  m3 = simulation.computeMessageLength(final);

  ofstream results1("results_general_2",ios::app);
  results1 << setw(10) << setprecision(6) << m1 << "\t";
  results1 << setw(10) << setprecision(6) << m2 << "\t";
  results1 << setw(10) << setprecision(6) << m3 << endl;
  results1.close();
  //delete(moving);
  //delete(fixed);

  clock_t c_end = clock();
  auto t_end = high_resolution_clock::now();

  cout << endl;
  cout << "CPU time used: " 
  << double(c_end-c_start)/(double)(CLOCKS_PER_SEC) << " secs." << endl;
  cout << "Wall clock time elapsed: "
  << duration_cast<seconds>(t_end-t_start).count() << " secs." << endl << endl;
  return 0;
}

