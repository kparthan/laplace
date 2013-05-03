#include "Simulation.h"
#include "Message.h"

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
                       iterations(parameters.iterations),
                       acceptance_probability(parameters.acceptance),
                       increment_translation(parameters.increment_translation), 
                       print_status(parameters.print)
{
  // convert the increment in rotaiton angle to radians
  increment_rotation_angle = parameters.increment_rotation_angle * PI / 180;
}

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
  if (print_status == PRINT_DETAIL) {
    cout << "RMSD: " << superimposer.rmsd() << endl;
  }
  cout << superimposer.getRotationAngle() * 180 / PI << endl;
  rotation_axis = superimposer.getRotationAxis();
  //cout << rotation_axis.l2Norm() << endl;
  //rotation_axis.print();
  //Matrix<double> rot = superimposer.getRotationMatrix();
  //rot.print();
  //cout << superimposer.rmsd() << endl;
  //fixed->undoLastSelection();
  //moving->undoLastSelection();
}

/*!
 *  \brief This function generates a random translation matrix.
 */
Matrix<double> Simulation::randomTranslation()
{
  // randomly choose a point on a unit sphere
  /* choose theta \in [0,PI] -- the angle with Z-axis */
  double theta = ((double)rand() * PI) / RAND_MAX;

  /* choose phi \in [0, 2 PI] -- the angle in the XY plane */
  double phi = ((double)rand() * 2 * PI) / RAND_MAX;

  Vector<double> direction(3);
  direction[0] = sin(theta) * cos(phi);
  direction[1] = sin(theta) * sin(phi);
  direction[2] = cos(theta);
  direction = direction * increment_translation;
  //direction.print();
  Matrix<double> translation_matrix = Matrix<double>::identity(4,4);
  translation_matrix[0][3] = direction[0];  
  translation_matrix[1][3] = direction[1];  
  translation_matrix[2][3] = direction[2];  

  // randomly choose the axis along which the structure is translated
  /*double random = rand() / (double) RAND_MAX;
  if (random <= 0.33) {
    // translate along x-axis
    translation_matrix[0][3] = move;
  } else if (random > 0.33 and random <= 0.67) {
    // translate along y-axis
    translation_matrix[1][3] = move;
  } else {
    // translate along z-axis
    translation_matrix[2][3] = move;
  }*/
  return translation_matrix;
}

/*!
 *  \brief This function perturbs the existing axis of rotation.
 */
void Simulation::perturbRotationAxis()
{
  array<double,2> angles = angleWithAxes(rotation_axis);

  // perturb theta
  double sign = rand() / RAND_MAX;
  if (sign < 0.5) {
    angles[0] -= increment_rotation_angle;
  } else {
    angles[0] += increment_rotation_angle;
  }

  // perturb phi 
  sign = rand() / RAND_MAX;
  if (sign < 0.5) {
    angles[1] -= increment_rotation_angle;
  } else {
    angles[1] += increment_rotation_angle;
  }

  double theta = angles[0];
  double phi = angles[1];

  // reconstruct the axis
  rotation_axis[0] = sin(theta) * cos(phi);
  rotation_axis[1] = sin(theta) * sin(phi);
  rotation_axis[2] = cos(theta);
}

/*!
 *  \brief This function generates a random rotation matrix.
 */
Matrix<double> Simulation::randomRotation()
{
  /*Vector<double> axis(3);

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
  double angle = ((double)rand() * 2 * PI) / RAND_MAX;*/
  Matrix<double> rotation_matrix = rotationMatrix(rotation_axis,
                                                  increment_rotation_angle);
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
 *  \return the ProteinStructure at the end of the simulation
 */
ProteinStructure Simulation::perturb()
{
  Matrix<double> transformation = Matrix<double>::identity(4,4);
  Matrix<double> current_transformation;
  double previous_deviation,current_deviation; 
  double initial_L1_deviation,final_L1_deviation;
  srand(time(NULL));

  vector<array<double,3>> coordinates;
  if (print_status == PRINT_DETAIL) {
    coordinates = moving->getAtomicCoordinates<double>();
    ofstream file("output/original_coordinates");
    for (int i=0; i<coordinates.size(); i++) {
      file << coordinates[i][0] << " " << coordinates[i][1] 
           << " " << coordinates[i][2] << endl;
    }
    file.close();
  }
  int accept = 0, accept_with_prob = 0, perturb_axis = 0, reject = 0;
  ProteinStructure moving_copy_persist = *moving;
  ProteinStructure moving_copy;
  previous_deviation = computeL1Deviation();
  initial_L1_deviation = previous_deviation;
  for (int i=0; i<iterations; i++) {
    // select translation or rotation randomly
    // choose a random number x between 0 and 1
    // if 0 < x <= 0.33 --> select translation
    // if 0.33 < x <= 0.67 --> perturb the rotation axis
    // if 0.67 < x <= 1 --> select rotation
    double random = rand() / (double) RAND_MAX;
    if (random <= 0.33) { 
      // random translation
      current_transformation = randomTranslation();
    } else if (random > 0.33 && random < 0.67) {
      // perturb the axis of rotation
      perturbRotationAxis();
      perturb_axis++;
      if (print_status == PRINT_DETAIL) {
        cout << "Iteration: " << i+1 << "\tPERTURBATION"
             << " OF AXIS" << endl;
      } 
      continue;
    } else {  
      // random rotation
      current_transformation = randomRotation();
    }

    moving_copy = moving_copy_persist;
    moving_copy.transform(current_transformation);
    current_deviation = computeL1Deviation(moving_copy);
    if (current_deviation < previous_deviation) {
      // accept the new configuration
      if (print_status == PRINT_DETAIL) {
        cout << "Iteration: " << i+1 << "\tACCEPT ("
             << "current_dev = " << current_deviation << ", "
             << "previous_dev = " << previous_deviation << ")" << endl;
      }
      accept++;
      moving_copy_persist = moving_copy;
      transformation = current_transformation * transformation ;
      previous_deviation = current_deviation;
    } else {
      // accept the new configuration with a probability
      random = rand() / (double) RAND_MAX;
      if (random <= acceptance_probability) {
        if (print_status == PRINT_DETAIL) {
          cout << "Iteration: " << i+1 << "\tACCEPT ("
               << "current_dev = " << current_deviation << ", "
               << "previous_dev = " << previous_deviation << ") "
               << "with probability " << acceptance_probability << endl;
        }
        accept_with_prob++;
        moving_copy_persist = moving_copy;
        transformation = current_transformation * transformation ;
        previous_deviation = current_deviation;
      } else {
        if (print_status == PRINT_DETAIL) {
          cout << "Iteration: " << i+1 << "\tSTATE UNCHANGED" << endl;
        }
        reject++;
      }
    }
  }
  if (print_status == PRINT_DETAIL) {
    cout << "\n# of acceptances: " << accept << endl;
    cout << "# of acceptances with a probability: " << accept_with_prob << endl;
    cout << "# of random perturbations of axis: " << perturb_axis << endl;
    cout << "# of rejections: " << reject << endl << endl;
    transformation.print();
    cout << "Initial L1 distance after superposition: " 
         << initial_L1_deviation / coordinates.size()  << endl;
    final_L1_deviation = computeL1Deviation(moving_copy_persist);
    cout << "Final L1 distance after perturbation: " 
         << final_L1_deviation / coordinates.size() << endl;

    /*moving->transform(transformation);
    coordinates = moving->getAtomicCoordinates<double>();
    ofstream file("output/after_transformation");
    for (int i=0; i<coordinates.size(); i++) {
      file << coordinates[i][0] << " " << coordinates[i][1] 
           << " " << coordinates[i][2] << endl;
    }
    file.close();
    coordinates = moving_copy_persist.getAtomicCoordinates<double>();
    ofstream fw2("output/after2");
    for (int i=0; i<coordinates.size(); i++) {
      fw2 << coordinates[i][0] << " " << coordinates[i][1] 
          << " " << coordinates[i][2] << endl;
    }
    fw2.close();*/
  }
  return moving_copy_persist;
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

/*!
 *  \brief This function returns the three sets of deviations with respect to
 *  the optimal superimposition using L2 norm.
 *  \return the set of deviations
 */
vector<array<double,3>> Simulation::getDeviations()
{
  ProteinStructure moving_copy = *moving;
  return getDeviations(moving_copy);
}

/*!
 *  \brief This function gets the three sets of deviations.
 *  \param protein a reference toa ProteinStructure
 *  \return the set of deviations
 */
vector<array<double,3>> Simulation::getDeviations(ProteinStructure &protein)
{
  vector<array<double,3>> deviations;
  vector<Atom> atoms_fixed = fixed->getAtoms();
  vector<Atom> atoms_protein = protein.getAtoms();
  assert(atoms_fixed.size() == atoms_protein.size());
  for (int i=0; i<atoms_fixed.size(); i++) {
    array<double,3> devs;
    Point<double> p1 = atoms_fixed[i].point<double>();
    Point<double> p2 = atoms_protein[i].point<double>();
    devs[0] = p2.x() - p1.x();
    devs[1] = p2.y() - p1.y();
    devs[2] = p2.z() - p1.z();
    deviations.push_back(devs);
  }
  return deviations;
}

/*!
 *  \brief This function computes the message lengths with respect to
 *  the deviations computed using the L2 norm.
 */
void Simulation::computeMessageLength()
{
  vector<array<double,3>> deviations = getDeviations();
  Message message(deviations);
  vector<array<double,2>> normal_estimates = message.getNormalEstimates();
  vector<double> msglen = message.encodeUsingNormalModel();
  if (print_status == PRINT_DETAIL) {
    cout << "NORMAL ESTIMATES:-\n";
    for (int i=0; i<1; i++) {
      //cout << "DEVIATION " << i+1 << ":\n";
      cout << "\tMean: " << normal_estimates[i][0] << endl;
      cout << "\tSigma: " << normal_estimates[i][1] << endl;
      cout << "\tMsg. length: " << msglen[i] << endl;
    }
  }
  cout << "Net message length: " << msglen[0] << endl;
}

/*!
 *  \brief This function computes the message lengths with respect to
 *  the deviations computed using the L1 norm.
 */
void Simulation::computeMessageLength(ProteinStructure &protein)
{
  vector<array<double,3>> deviations = getDeviations(protein);
  Message message(deviations);
  vector<array<double,2>> laplace_estimates = message.getLaplaceEstimates();
  vector<double> msglen = message.encodeUsingLaplaceModel();
  if (print_status == PRINT_DETAIL) {
    cout << "\nLAPLACE ESTIMATES:-\n";
    for (int i=0; i<1; i++) {
      //cout << "DEVIATION " << i+1 << ":\n";
      cout << "\tMedian: " << laplace_estimates[i][0] << endl;
      cout << "\tScale: " << laplace_estimates[i][1] << endl;
      cout << "\tMsg. length: " << msglen[i] << endl;
    }
  }
  cout << "Net message length: " << msglen[0] << endl;
}

