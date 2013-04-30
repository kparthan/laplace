#ifndef SIMULATION_H
#define SIMULATION_H

#include "Header.h"

class Simulation
{
  private:
    //! The protein that is being perturbed
    ProteinStructure *moving;

    //! A fixed protein structure
    ProteinStructure *fixed;

    //! Number of iterations
    int iterations;

    //! Acceptance probability
    double acceptance_probability;

    //! Length of the move during random translation
    double step;

    //! Verbose flag
    int print_status;

    //! Generates a random translation matrix
    Matrix<double> randomTranslation(double);

    //! Generates a random rotation matrix
    Matrix<double> randomRotation();

    //! Calculates the L1-norm
    double L1Norm(Point<double> &, Point<double> &);

  public:
    //! Null constructor
    Simulation();

    //! Constructor
    Simulation(ProteinStructure *, ProteinStructure *);

    //! Superimpose the two structures
    void initialSuperposition();

    //! Perturb the protein structure
    void perturb(int, double, double);

    //! Computes the L1 distance between the fixed and moving 
    //! protein structures
    double computeL1Deviation();

    //! Computes the L1 distance between two protein structures
    double computeL1Deviation(ProteinStructure &);
};

#endif

