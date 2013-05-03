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
#include <liblcb/liblcb.h>
#include <boost/math/constants/constants.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#define DEFAULT_ITERATIONS 100
#define DEFAULT_ACCEPTANCE_PROBABILITY 0.2
#define DEFAULT_INCREMENT_TRANSLATION 0.05
#define DEFAULT_INCREMENT_ROTATION_ANGLE 0.05

#define AOM 0.001
#define PI boost::math::constants::pi<double>()
#define ZERO std::numeric_limits<double>::epsilon()

#define PRINT_DETAIL 1
#define PRINT_NON_DETAIL 0

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;
using namespace lcb;
using namespace lcb::geometry;

#endif

