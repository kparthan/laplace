#ifndef SIMULATION_H
#define SIMULATION_H

#include "Support.h"

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

    //! Increment during random translation
    double increment_translation;

    //! Increment (in degrees) to the angle of 
    double increment_rotation_angle;

    //! Verbose flag
    int print_status;

    //! Method to encode deviations
    int num_deviations_sets;

    //! Rotation axis
    Vector<double> rotation_axis;

    //! Generates a random translation matrix
    Matrix<double> randomTranslation();

    //! Perturbs the axis of rotation
    void perturbRotationAxis();

    //! Generates a random rotation matrix
    Matrix<double> randomRotation();

    //! Calculates the L1-norm
    double L1Norm(Point<double> &, Point<double> &);

  public:
    //! Null constructor
    Simulation();

    //! Constructor
    Simulation(ProteinStructure *, ProteinStructure *, struct Parameters &);

    //! Superimpose the two structures
    void initialSuperposition();

    //! Perturb the protein structure
    ProteinStructure perturb();

    //! Computes the L1 distance between the fixed and moving 
    //! protein structures
    double computeL1Deviation();

    //! Computes the L1 distance between two protein structures
    double computeL1Deviation(ProteinStructure &);

    //! Computes the set of deviations
    vector<array<double,3>> getDeviations();

    //! Computes the set of deviations
    vector<array<double,3>> getDeviations(ProteinStructure &);

    //! Computes the message length of the deviations (L2 Norm)
    void computeMessageLength();

    //! Computes the message length of the deviations (L1 Norm)
    void computeMessageLength(ProteinStructure &);

};

#endif

