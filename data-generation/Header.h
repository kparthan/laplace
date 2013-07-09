#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <ctime>
#include <thread>
#include <chrono>
#include <boost/math/constants/constants.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#define DEFAULT_SAMPLES 100
#define DEFAULT_MEAN 0.0
#define DEFAULT_SCALE 2.0
#define DEFAULT_NOISE_SIGMA 0
#define DEFAULT_ITERATIONS 1
#define DEFAULT_GENERATE_DISTRIBUTION "laplace"
#define DEFAULT_ESTIMATE_DISTRIBUTION "laplace"

#define AOM 0.001
#define SET 1
#define UNSET 0

#define PI boost::math::constants::pi<double>()
#define ZERO std::numeric_limits<double>::epsilon()

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;

#endif

