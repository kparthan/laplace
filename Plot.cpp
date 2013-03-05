#include "Plot.h"

/*!
 *  \brief Null constructor: creates a Gnuplot script with
 *  default graph settings.
 */
Plot::Plot()
{
	//	default specifications
	script.open("temp/plotScript.p");
	script << "# Gnuplot script file for plotting data in file \"data\"\n\n";
	script << "set terminal post eps" << endl;
	script << "set autoscale\t";
	script << "# scale axes automatically" << endl;
	script << "set xtic auto\t";
	script << "# set xtics automatically" << endl;
	script << "set ytic auto\t";
	script << "# set ytics automatically" << endl;
	script.close();
}

/*!
 *  \brief The module makes changes to the script file to include the
 *	graph title and axes labels
 *	\param labels a vector of strings
 */
void Plot::label(vector<string> &labels)
{
	script.open("temp/plotScript.p",ios::app);
	script << "set title \"" << labels[0] << "\"" << endl;
	script << "set xlabel \"" << labels[1] << "\"" << endl;
	script << "set ylabel \"" << labels[2] << "\"" << endl;
	script.close();
}

/*!
 *  \brief The module makes changes to the script file to include the
 *	graph title and axes labels
 *	\param s string
 *	\param labels a vector of strings
 */
void Plot::label(string s, vector<string> &labels)
{
	script.open("temp/plotScript.p",ios::app);
	script << "set title \"" << labels[0] << "\"" << endl;
  	script << "set label \"" << s << "\" at graph 0.005, graph 0.95 "
  	"font \",10\"" << endl;
	script << "set xlabel \"" << labels[1] << "\"" << endl;
	script << "set ylabel \"" << labels[2] << "\"" << endl;
	script.close();
}

/*!
 *	\brief The module makes changes to the script file to include the
 *  ranges of X and Y axes
 *  \param xrange a std::pair
 *  \param yrange a std::pair
 */
void Plot::setRange(pair<double,double> xrange, pair<double,double> yrange)
{
	script.open("temp/plotScript.p",ios::app);
	script << "set xr [" << xrange.first - GAP << ":" << xrange.second + GAP  << "]"  << endl;
	script << "set yr [" << yrange.first - GAP/2 << ":" << yrange.second + GAP/2 << "]"  << endl;
	script.close();
}

/*!
 *	\brief This function is used to plot the generated X values
 *  	\param randomData a reference to a vector<double>
 */
void Plot::sketch(vector<double> &randomData)
{
	ofstream dataFile;
	dataFile.open("temp/data.txt");
	int numPoints = randomData.size();

	for(int i=0; i<numPoints; i++) {
		dataFile << i+1 << "\t" << randomData[i] << endl;
	}
	dataFile.close();

	script.open("temp/plotScript.p",ios::app);
	script << "set output \"temp/file_X.eps\"" << endl;
	script << "plot \"temp/data.txt\" using 1:2 title 'random x' with points "
	"lc rgb \"red\"" << endl;
	script.close();

	system("gnuplot -persist temp/plotScript.p");	
}

/*!
 *	\brief This function is used to plot the generated X values against
 *	the corresponding function values 
 *  	\param xVal a reference to a vector<double>
 *  	\param fxVal a reference to a vector<double>
 */
void Plot::sketch(vector<double> &xVal, vector<double> &fxVal)
{
	if(xVal.size() != fxVal.size()) {
		printError("Number of points mismatch!");
	}
	int numPoints = xVal.size();
	ofstream dataFile;
	dataFile.open("temp/data_XfX.txt");

	for(int i=0; i<numPoints; i++) {
		dataFile << xVal[i] << "\t" << fxVal[i] << endl;
	}
	dataFile.close();

	script.open("temp/plotScript.p",ios::app);
	script << "set output \"temp/file_XfX.eps\"" << endl;
	script << "plot \"temp/data_XfX.txt\" using 1:2 title 'f(x)' with points "
	"lc rgb \"red\"" << endl;
	script.close();

	system("gnuplot -persist temp/plotScript.p");	
}

/*!
 *	\brief This function is used to plot the generated X values against
 *	the corresponding function values and the generated values
 *  	\param xVal a reference to a vector<double>
 *  	\param fxVal a reference to a vector<double>
 *  	\param yVal a reference to a vector<double>
 */
void Plot::sketch(vector<double> &xVal, vector<double> &fxVal, vector<double> &yVal)
{
	if(xVal.size() != fxVal.size()) {
		printError("Number of points mismatch!");
	}
	int numPoints = xVal.size();
	if(numPoints != yVal.size()) {
		printError("Number of points mismatch!");
	}
	ofstream dataFile;
	dataFile.open("temp/data_XY.txt");

	for(int i=0; i<numPoints; i++) {
		dataFile << xVal[i] << "\t" << fxVal[i] << "\t";
		dataFile << yVal[i] << endl;
	}
	dataFile.close();

	script.open("temp/plotScript.p",ios::app);
	//script << "set nokey" << endl;
	script << "set output \"temp/file_XY.eps\"" << endl;
	script << "set multiplot" << endl;
	script << "plot \"temp/data_XY.txt\" using 1:2 title 'f(x)' \\" << endl;
	script << "with points lc rgb \"red\", \\" << endl;
	script << "\"temp/data_XY.txt\" using 1:3 title 'y' \\" << endl;
	script << "with points lc rgb \"blue\"" << endl;
	script.close();

	system("gnuplot -persist temp/plotScript.p");	
}

