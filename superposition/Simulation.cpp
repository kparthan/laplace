#include "Simulation.h"
#include <stdio.h>

/*!
 *  \brief A null constructor.
 */
Simulation::Simulation()
{}

/*!
 *  \brief This is a constructor module to instantiate the object.
 *  \param moving a pointer to a ProteinStructure
 *  \param fixed a pointer to a ProteinStructure
 *  \param parameters a reference to a struct Parameters
 */
Simulation::Simulation(ProteinStructure *moving, ProteinStructure *fixed,
                       struct Parameters &parameters) :
                       moving(moving), fixed(fixed),
{}

/*!
 *  \brief This function superimposes the moving protein onto the
 *  fixed one and brings the moving protein into the minimum configuration
 *  state.
 */
void Simulation::initialSuperposition()
{
  Superimposer superimposer;
  superimposer.superimpose(moving).onto(fixed);
  moving->transform(superimposer.getRotationMatrix());
  //Matrix<double> rot = superimposer.getRotationMatrix();
  //rot.print();
  //cout << superimposer.rmsd() << endl;
  //fixed->undoLastSelection();
  //moving->undoLastSelection();
}

/*!
 *  \brief This function generates a random translation matrix.
 *  \param move a double
 */
Matrix<double> Simulation::randomTranslation(double move)
{
  Matrix<double> translation_matrix = Matrix<double>::identity(4,4);
  // randomly choose the axis along which the structure is translated
  double random = rand() / (double) RAND_MAX;
  if (random <= 0.33) {
    // translate along x-axis
    translation_matrix[0][3] = move;
  } else if (random > 0.33 and random <= 0.67) {
    // translate along y-axis
    translation_matrix[1][3] = move;
  } else {
    // translate along z-axis
    translation_matrix[2][3] = move;
  }
  return translation_matrix;
}

/*!
 *  \brief This function generates a random rotation matrix.
 */
Matrix<double> Simulation::randomRotation()
{
  Vector<double> axis(3);

  // randomly choose the axis about which the structure is rotated
  double random = rand() / (double) RAND_MAX;
  if (random <= 0.33) {
    // rotate about x-axis
    axis[0] = 1;
  } else if (random > 0.33 and random <= 0.67) {
    // rotate along y-axis
    axis[1] = 1;
  } else {
    // rotate along z-axis
    axis[2] = 1;
  }
  // randomly select an angle in [0,2 PI]
  double angle = ((double)rand() * 2 * PI) / RAND_MAX;
  Matrix<double> rotation_matrix = rotationMatrix(axis,angle);
  rotation_matrix.changeDimensions(4,4);
  rotation_matrix[3][3] = 1;
  return rotation_matrix;
}

/*!
 *  \brief This function calculates the L1-norm between any two points.
 *  \param p1 a reference to a Point<double>
 *  \param p2 a reference to a Point<double>
 *  \return the L1-norm
 */
double Simulation::L1Norm(Point<double> &p1, Point<double> &p2)
{
  double distance = 0;
  distance += fabs(p1.x() - p2.x());
  distance += fabs(p1.y() - p2.y());
  distance += fabs(p1.z() - p2.z());
  return distance;
}

/*!
 *  \brief This function performs the perturbation in accordance with the
 *  Monte Carlo simulation.
 *  \param iterations an integer
 *  \param acceptance a double
 *  \param move a double
 */
void Simulation::perturb(int iterations, double acceptance, double move)
{
  Matrix<double> transformation = Matrix<double>::identity(4,4);
  Matrix<double> current_transformation;
  double previous_deviation,current_deviation; 
  srand(time(NULL));

  vector<array<double,3>> c = moving->getAtomicCoordinates<double>();
  ofstream fw1("original");
  for (int i=0; i<c.size(); i++) {
    fw1 << c[i][0] << " " << c[i][1] << " " << c[i][2] << endl;
  }
  fw1.close();
  int count = 0;
  ProteinStructure moving_copy_persist = *moving;
  ProteinStructure moving_copy;
  previous_deviation = computeL1Deviation();
  for (int i=0; i<iterations; i++) {
    // select translation or rotation randomly
    // choose a random number x between 0 and 1
    // if 0 < x <= 0.5 --> select translation
    // if 0.5 < x <= 1 --> select rotation
    double random = rand() / (double) RAND_MAX;
    if (random <= 0.5) { // random translation
      current_transformation = randomTranslation(move);
    } else {  // random rotation
      current_transformation = randomRotation();
    }

    moving_copy = moving_copy_persist;
    moving_copy.transform(current_transformation);
    current_deviation = computeL1Deviation(moving_copy);
    if (current_deviation < previous_deviation) {
      // accept the new configuration
      count++;
      moving_copy_persist = moving_copy;
      transformation = current_transformation * transformation ;
      previous_deviation = current_deviation;
    } else {
      // accept the new configuration with a probability
      random = rand() / (double) RAND_MAX;
      if (random <= acceptance) {
        count++;
        moving_copy_persist = moving_copy;
        transformation = current_transformation * transformation ;
        previous_deviation = current_deviation;
      }
    }
    cout << i << endl;
  }
  printf("M1: %p\nM2: %p\n", moving, &moving_copy_persist);
  cout << "Count: " << count << endl;
  transformation.print();
  //moving->transform(transformation);
  c = moving->getAtomicCoordinates<double>();
  ofstream fw3("after1");
  for (int i=0; i<c.size(); i++) {
    fw3 << c[i][0] << " " << c[i][1] << " " << c[i][2] << endl;
  }
  fw3.close();
  //delete moving;
  //moving = new ProteinStructure(*moving_copy_persist);
  c = moving_copy_persist.getAtomicCoordinates<double>();
  ofstream fw2("after2");
  for (int i=0; i<c.size(); i++) {
    fw2 << c[i][0] << " " << c[i][1] << " " << c[i][2] << endl;
  }
  fw2.close();
}

/*!
 *  \brief This function computes the L1 distance between the fixed and
 *  moving protein structures.
 *  \return the L1 distance
 */
double Simulation::computeL1Deviation()
{
  ProteinStructure moving_copy = *moving;
  return computeL1Deviation(moving_copy);
}

/*!
 *  \brief This function computes the L1 distance between the current
 *  any protein structure and the fixed one.
 */
double Simulation::computeL1Deviation(ProteinStructure &protein)
{
  double deviation = 0;
  vector<Atom> atoms_fixed = fixed->getAtoms();
  vector<Atom> atoms_protein = protein.getAtoms();
  assert(atoms_fixed.size() == atoms_protein.size());
  for (int i=0; i<atoms_fixed.size(); i++) {
    Point<double> p1 = atoms_fixed[i].point<double>();
    Point<double> p2 = atoms_protein[i].point<double>();
    deviation += L1Norm(p1,p2);
  }
  return deviation;
}

