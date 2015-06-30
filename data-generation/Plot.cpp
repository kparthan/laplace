#include "Plot.h"

/*!
 *  \brief Null constructor: creates a Gnuplot script with
 *  default graph settings.
 */
Plot::Plot()
{}

/*!
 *  \brief This is a constructor function.
 *  \param file_name a reference to a string
 */
Plot::Plot(string &script_file) : script_file(script_file)
{
  script.open(script_file.c_str());
  script.close();
  specifyDefault();
}

/*!
 *  \brief Default specifications to be printed out to the script file.
 */
void Plot::specifyDefault()
{
	script.open(script_file.c_str(),ios::app);
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
	script.open(script_file.c_str(),ios::app);
	script << "set title \"" << labels[0] << "\"" << endl;
	script << "set xlabel \"" << labels[1] << "\\n\"" << endl;
	script << "set ylabel \"" << labels[2] << "\\n\"" << endl;
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
	script.open(script_file.c_str(),ios::app);
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
	script.open(script_file.c_str(),ios::app);
	script << "set xr [" << xrange.first - GAP << ":" << xrange.second + GAP 
         << "]"  << endl;
	script << "set yr [" << yrange.first << ":" << yrange.second 
         << "]"  << endl;
	//script << "set yr [" << yrange.first - GAP << ":" << yrange.second + GAP 
  //       << "]"  << endl;
	script.close();
}

/*!
 *  \brief This function plots the predictions
 *  \param data_file a reference to a string
 *  \param predictions a reference to a vector<vector<double>>
 */
void Plot::sketch(string &data_file, vector<vector<double>> &predictions)
{
  script.open(script_file.c_str(),ios::app);
  script << "set output \"results/plots/" << data_file << ".eps\"" << endl;
	script << "set multiplot" << endl;
  script << "set xtics font \"Times-Roman, 25\"\n";
  script << "set ytics font \"Times-Roman, 25\"\n";
  script << "set xlabel font \"Times-Roman, 35\"\n";
  script << "set ylabel font \"Times-Roman, 35\"\n";
  script << "set key font \",25\"\n";
  script << "set key spacing 1.5\n";
  script << "plot \"results/data/" << data_file << "\" using 1:2 title "
         << "'original' with points lc rgb \"red\", \\" << endl;
  script << "\"results/data/" << data_file << "\" using 1:3 title "
         << "'Gaussian' with points lc rgb \"blue\", \\" << endl;
  script << "\"results/data/" << data_file << "\" using 1:4 title "
         << "'Laplace' with points lc rgb \"dark-green\"" << endl;
	script.close();

	system("gnuplot -persist results/script.plot");	
}

/*!
 *
 */
void Plot::sketchStatistics(string &data_file)
{
  script.open(script_file.c_str(),ios::app);
  script << "set output \"results/plots/" << data_file << ".eps\"" << endl;
	script << "set multiplot" << endl;
  script << "plot \"results/data/" << data_file << "\" using (column(0)):5 title "
         << "'normal message length' with lines lc rgb \"red\", \\" << endl;
  script << "\"results/data/" << data_file << "\" using (column(0)):10 title "
         << "'laplace message length' with lines lc rgb \"blue\"" << endl;
	script.close();

	system("gnuplot -persist results/script.plot");	
}

/*!
 *	\brief This function is used to plot the generated X values
 * 	\param randomData a reference to a vector<double>
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

