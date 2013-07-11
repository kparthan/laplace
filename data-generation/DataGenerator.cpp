#include "DataGenerator.h"
#include "NormalDataGenerator.h"
#include "LaplaceDataGenerator.h"
#include "Plot.h"
#include "Message.h"

/*!
 *  \brief This is a null constructor.
 */
DataGenerator::DataGenerator() : type(DEFAULT)
{}

/*!
 *  \brief Constructor
 *  \param parameters a reference to a struct Parameters
 */
DataGenerator::DataGenerator(struct Parameters &parameters):
                             parameters(parameters), type(DEFAULT)
{}

/*!
 *  \brief This function sorts the elements in the list
 *  \param list a reference to a vector<double>
 *  \return the sorted list
 */
vector<double> DataGenerator::sort(vector<double> &list)
{
  int num_samples = list.size();
	vector<double> sortedList(list);
  vector<int> index(num_samples,0);
	for(int i=0; i<num_samples; i++) {
			index[i] = i;
  }
	quicksort(sortedList,index,0,num_samples-1);
  return sortedList;
}

/*!
 *  This is an implementation of the classic quicksort() algorithm to sort a
 *  list of data values. The module uses the overloading operator(<) to 
 *  compare two Point<T> objects. 
 *  Pivot is chosen as the right most element in the list(default)
 *  This function is called recursively.
 *  \param list a reference to a vector<double>
 *	\param index a reference to a vector<int>
 *  \param left an integer
 *  \param right an integer
 */
void DataGenerator::quicksort(vector<double> &list, vector<int> &index, 
                              int left, int right)
{
	if(left < right)
	{
		int pivotNewIndex = partition(list,index,left,right);
		quicksort(list,index,left,pivotNewIndex-1);
		quicksort(list,index,pivotNewIndex+1,right);
	}
}

/*!
 *  This function is called from the quicksort() routine to compute the new
 *  pivot index.
 *  \param list a reference to a vector<double>
 *	\param index a reference to a vector<int>
 *  \param left an integer
 *  \param right an integer
 *  \return the new pivot index
 */
int DataGenerator::partition(vector<double> &list, vector<int> &index,
                             int left, int right)
{
	double temp,pivotPoint = list[right];
	int storeIndex = left,temp_i;
	for(int i=left; i<right; i++) {
		if(list[i] < pivotPoint) {
			temp = list[i];
			list[i] = list[storeIndex];
			list[storeIndex] = temp;
			temp_i = index[i];
			index[i] = index[storeIndex];
			index[storeIndex] = temp_i;
			storeIndex += 1;	
		}
	}
	temp = list[storeIndex];
	list[storeIndex] = list[right];
	list[right] = temp;
	temp_i = index[storeIndex];
	index[storeIndex] = index[right];
	index[right] = temp_i;
	return storeIndex;
}

/*!
 *  \brief This function generates the relevant file name to which the data 
 *  needs to be added.
 *  \param name a pointer to a char 
 *  \param num_samples an integer
 *  \param scale_index an integer 
 *  \return the file name
 */
string DataGenerator::getFileName(const char *name, int num_samples,
                                  int scale_index)
{
  string file = name;
  file += "_n_" + boost::lexical_cast<string>(num_samples);
  double mean = parameters.mean;
  file += "_mean_" + boost::lexical_cast<string>(mean);
  double scale = parameters.scale[scale_index];
  file += "_scale_" + boost::lexical_cast<string>(scale).substr(0,3);
  //cout << file << endl;
  return file;
}

/*!
 *  \brief This function outputs the data to a file.
 *  \param file_name a reference to a string 
 *  \param num_samples an integer
 *  \param scale_index an integer 
 *  \param estimates a reference to a struct Estimates
 */
void DataGenerator::updateResults(string &file_name, int num_samples,
                              int scale_index, struct Estimates &estimates)
{
  string file = "results/data/statistics_" + file_name; 
  ofstream fp(file.c_str(),ios::app);
  /*fp << setw(10) << num_samples << "\t";
  fp << setw(5) << setprecision(3) << parameters.mean << "\t";
  fp << setw(5) << setprecision(3) << parameters.scale[scale_index] << "\t";
  fp << setw(10) << parameters.distribution << "\t";*/

  // print normal estimates
  fp << setw(10) << setprecision(3) << estimates.normal_mean << "\t";
  fp << setw(10) << setprecision(3) << estimates.normal_sigma_ml << "\t";
  fp << setw(10) << setprecision(3) << estimates.normal_sigma_mml << "\t";
  fp << fixed << setw(10) << setprecision(3) << estimates.normal_msglen << "\t";

  // print laplace estimates
  fp << setw(10) << setprecision(3) << estimates.laplace_mean << "\t";
  fp << setw(10) << setprecision(3) << estimates.laplace_scale_ml << "\t";
  fp << setw(10) << setprecision(3) << estimates.laplace_scale_mml << "\t";
  fp << fixed << setw(10) << setprecision(3) << estimates.laplace_msglen << "\t";

  // print winner
  if (estimates.normal_msglen < estimates.laplace_msglen) {
    fp << setw(10) << "Normal";
  } else if (estimates.normal_msglen > estimates.laplace_msglen) {
    fp << setw(10) << "Laplace";
  } else {
    fp << setw(10) << "Draw";
  }

  /*  error = estimate - true */
  // print difference in normal estimates
  double normal_mean_error = estimates.normal_mean - parameters.mean; 
  double normal_ml_sigma_error = estimates.normal_sigma_ml - parameters.scale[scale_index];
  double normal_mml_sigma_error = estimates.normal_sigma_mml - parameters.scale[scale_index];
  fp << setw(10) << setprecision(3) << normal_mean_error << "\t";
  fp << setw(10) << setprecision(3) << normal_ml_sigma_error << "\t";
  fp << fixed << setw(10) << setprecision(3) << normal_mml_sigma_error << "\t";

  // print difference in laplace estimates
  double laplace_mean_error = estimates.laplace_mean - parameters.mean; 
  double laplace_ml_scale_error = estimates.laplace_scale_ml - parameters.scale[scale_index];
  double laplace_mml_scale_error = estimates.laplace_scale_mml - parameters.scale[scale_index];
  fp << setw(10) << setprecision(3) << laplace_mean_error << "\t";
  fp << setw(10) << setprecision(3) << laplace_ml_scale_error << "\t";
  fp << fixed << setw(10) << setprecision(3) << laplace_mml_scale_error << "\t";

  // calculate the difference in msg len
  double diff_msglen;
  if (type == LAPLACE) {
    diff_msglen = estimates.normal_msglen - estimates.laplace_msglen;
  } else if (type == NORMAL) {
    diff_msglen = estimates.laplace_msglen - estimates.normal_msglen;
  }
  fp << fixed << setw(10) << setprecision(3) << diff_msglen << "\t";
  fp << endl;
  fp.close();
}

/*!
 *  \brief This function updates the statistics over all iterations
 *  \param n an integer
 *  \param estimates a reference to a struct Estimates
 *  \param statistics a reference to a struct Statistics
 */
void DataGenerator::updateStatistics(int n, struct Estimates &estimates,
                                     struct Statistics &statistics)
{
  if (n == 1) {
    statistics.normal_sigma = vector<double>(6,0);
    statistics.laplace_scale = vector<double>(6,0);
    for (int i=0; i<3; i++) {
      statistics.normal_sigma[i] = estimates.normal_sigma_ml;
      statistics.normal_sigma[i+3] = estimates.normal_sigma_mml;
      statistics.laplace_scale[i] = estimates.laplace_scale_ml;
      statistics.laplace_scale[i+3] = estimates.laplace_scale_mml;
    }
  } else {
    double sum;
    // update normal statistics
    {
      // update ML average
      sum = (n-1) * statistics.normal_sigma[0];
      statistics.normal_sigma[0] = (sum + estimates.normal_sigma_ml) / n;
      // update ML min
      if (statistics.normal_sigma[1] > estimates.normal_sigma_ml) {
        statistics.normal_sigma[1] = estimates.normal_sigma_ml;
      }
      // update ML max
      if (statistics.normal_sigma[2] < estimates.normal_sigma_ml) {
        statistics.normal_sigma[2] = estimates.normal_sigma_ml;
      }
      // update MML average
      sum = (n-1) * statistics.normal_sigma[3];
      statistics.normal_sigma[3] = (sum + estimates.normal_sigma_mml) / n;
      // update MML min
      if (statistics.normal_sigma[4] > estimates.normal_sigma_mml) {
        statistics.normal_sigma[4] = estimates.normal_sigma_mml;
      }
      // update MML max
      if (statistics.normal_sigma[5] < estimates.normal_sigma_mml) {
        statistics.normal_sigma[5] = estimates.normal_sigma_mml;
      }
    }
    // update laplace statistics
    {
      // update ML average
      sum = (n-1) * statistics.laplace_scale[0];
      statistics.laplace_scale[0] = (sum + estimates.laplace_scale_ml) / n;
      // update ML min
      if (statistics.laplace_scale[1] > estimates.laplace_scale_ml) {
        statistics.laplace_scale[1] = estimates.laplace_scale_ml;
      }
      // update ML max
      if (statistics.laplace_scale[2] < estimates.laplace_scale_ml) {
        statistics.laplace_scale[2] = estimates.laplace_scale_ml;
      }
      // update MML average
      sum = (n-1) * statistics.laplace_scale[3];
      statistics.laplace_scale[3] = (sum + estimates.laplace_scale_mml) / n;
      // update MML min
      if (statistics.laplace_scale[4] > estimates.laplace_scale_mml) {
        statistics.laplace_scale[4] = estimates.laplace_scale_mml;
      }
      // update MML max
      if (statistics.laplace_scale[5] < estimates.laplace_scale_mml) {
        statistics.laplace_scale[5] = estimates.laplace_scale_mml;
      }
    }
  }
}

/*!
 *  \brief This function outputs the predictions to a file.
 *  \param file_name a reference to a string
 *  \param x a reference to a vector<double>
 *  \param fx a reference to a vector<double>
 *  \param predictions a reference to a vector<vector<double>>
 */
void DataGenerator::writeToFile(string &file_name,
                                vector<double> &x, vector<double> &fx, 
                                vector<vector<double>> &predictions)
{
  string file = "results/data/" + file_name;
  ofstream fp(file.c_str());
  for (int i=0; i<x.size(); i++) {
    fp << x[i] << "\t" << fx[i] << "\t";
    fp << predictions[0][i] << "\t" << predictions[1][i] << endl;
  }
  fp.close();
}

/*!
 *  \brief This function plots the predictions for a given parameter set
 *  \param data_file a reference to a string
 */
void DataGenerator::plotPredictions(string &data_file)
{
  string script_file("results/script.plot");
  Plot graph(script_file);
  pair<double,double> xrange,yrange;

  vector<string> labels(3,"");
  labels[0] = "";
  labels[1] = "x";
  labels[2] = "probability density";
  graph.label(labels);
  xrange = extremum(x);
  //yrange = extremum(predictions[0]);
  //xrange = pair<double,double>(-10,10);
  yrange = pair<double,double>(0,0.8);
  graph.setRange(xrange,yrange);
  graph.sketch(data_file,predictions);
}

/*!
 *  \brief This function estimates the message length and generates the plots.
 *  \param name a pointer to a char 
 *  \param list a reference to a vector<double>
 *  \param scale_index an integer 
 *  \return the normal & laplace estimates of the data 
 */
Estimates DataGenerator::estimateAndPlotModel(const char *name,
                                        vector<double> &list, 
                                        int scale_index)
{
  x = sort(list);
  fx = computeFunctionValues(x);
  struct Estimates estimates = mmlEstimate(x);
  predictions = predict(x,estimates);

  string file_name = getFileName(name,list.size(),scale_index);
  if (parameters.iterations == 1) {
    writeToFile(file_name,x,fx,predictions);
    plotPredictions(file_name);
  } else {
    updateResults(file_name,list.size(),scale_index,estimates);
  }

  if (estimates.normal_msglen < estimates.laplace_msglen) {
    estimates.winner = NORMAL; // normal wins
  } else if (estimates.normal_msglen > estimates.laplace_msglen) {
    estimates.winner = LAPLACE; // laplace wins
  } else {
    estimates.winner = DEFAULT; // draw
  }
  return estimates;
}

/*!
 *  \brief This function is used to plot the message lengths for both
 *  distributions over several iterations.
 *  \param name a pointer to a const char
 *  \param num_samples an integer
 *  \param scale_index an integer
 */
void DataGenerator::plotMessageLength(const char *name, int num_samples,
                                      int scale_index)
{
  string file_name = getFileName(name,num_samples,scale_index);
  file_name = "statistics_" + file_name;
  string script_file("results/script.plot");
  Plot graph(script_file);
  pair<double,double> xrange,yrange;

  vector<string> labels(3,"");
  labels[0] = "";
  labels[1] = "Iterations";
  labels[2] = "Message length (in bits)";
  graph.label(labels);
  //xrange = extremum(x);
  //yrange = extremum(predictions[0]);
  //xrange = pair<double,double>(1,101);
  //yrange = pair<double,double>(6400,6900);
  //graph.setRange(xrange,yrange);
  graph.sketchStatistics(file_name);
}

/*!
 *  \brief This function saves the statistics to the file
 *  \param statistics a reference to a struct Statistics
 *  \param iterations an integer
 *  \param sample_index an integer
 *  \param scale_index an integer
 */
void DataGenerator::saveErrorStatistics(struct Statistics &statistics,
                                        int iterations, int sample_index,
                                        int scale_index)
{
  double scale = parameters.scale[scale_index];
  string file_name = "results/data/";
  file_name += "errors_i_" + boost::lexical_cast<string>(iterations) + 
               "_s_" + boost::lexical_cast<string>(scale).substr(0,3);
  ofstream file(file_name.c_str(),ios::app);
  file << fixed << setw(10) << parameters.samples[sample_index];
  for (int i=0; i<statistics.normal_sigma.size(); i++) {
    file << fixed << setw(10) << setprecision(3) << statistics.normal_sigma[i];
  }
  for (int i=0; i<statistics.laplace_scale.size(); i++) {
    file << fixed << setw(10) << setprecision(3) << statistics.laplace_scale[i];
  }
  file << endl;
  file.close();
}

/*!
 *  \brief This method plots the errors in parameter estimates.
 */
void DataGenerator::plotErrors()
{
  for (int i=0; i<parameters.scale.size(); i++) {
    double scale = parameters.scale[i];
    string data_file = "errors_i_" + boost::lexical_cast<string>(parameters.iterations)
                       + "_s_" + boost::lexical_cast<string>(scale).substr(0,3);
    ofstream script("results/script.plot");
    script << "set term post eps" << endl;
    script << "set title \"Errors in Parameter Estimation\"" <<  endl;
    script << "set xlabel \"# of samples\"" << endl;
    script << "set ylabel \"Scale Parameter\"" << endl;
    script << "set output \"results/plots/" << data_file << ".eps\"" << endl;
    script << "set multiplot" << endl;
    script << "plot \"results/data/" << data_file << "\" using 1:2:($3):($4) title " 
           << "'normal ML' with errorbars lc rgb \"blue\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:5:($6):($7) title " 
           << "'normal MML' with errorbars lc rgb \"green\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:8:($9):($10) title " 
           << "'laplace ML' with errorbars lc rgb \"red\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:11:($12):($13) title " 
           << "'laplace MML' with errorbars lc rgb \"black\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:2 notitle " 
           << "with lines lc rgb \"blue\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:5 notitle " 
           << "with lines lc rgb \"green\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:8 notitle " 
           << "with lines lc rgb \"red\", \\" << endl;
    script << "\"results/data/" << data_file << "\" using 1:11 notitle " 
           << "with lines lc rgb \"black\"" << endl;
    script.close();
	  system("gnuplot -persist results/script.plot");	
  }
}

/*!
 *  \brief This function predicts the distribution with respect to the
 *  estimated parameters.
 *  \param x a reference to a vector<double>
 *  \param estimates a reference to a struct Estimates
 *  \return the list of estimated values
 */
vector<vector<double>> DataGenerator::predict(vector<double> &x, 
                                      struct Estimates &estimates)
{
  vector<vector<double>> y;

  // Estimate values using Normal distribution
  NormalDataGenerator normal(estimates.normal_mean,estimates.normal_sigma_mml);
  vector<double> y0 = normal.computeFunctionValues(x);
  y.push_back(y0);

  // Estimate values using Laplace distribution
  LaplaceDataGenerator laplace(estimates.laplace_mean,estimates.laplace_scale_mml);
  vector<double> y1 = laplace.computeFunctionValues(x);
  y.push_back(y1);

  return y;
}

/*!
 *  \brief This function computes the MML estimates.
 *  \param x a reference to a vector<double>
 *  \return the MML estimates of the parameters and the corresponding
 *  message length
 */
struct Estimates DataGenerator::mmlEstimate(vector<double> &x)
{
  Message message(x);
  message.estimateParameters();
  struct Estimates estimates;

  // Normal estimates
  //cout << "*** NORMAL ***" << endl;
  vector<double> normalEstimates = message.getNormalEstimates();
  estimates.normal_mean = normalEstimates[0];
  estimates.normal_sigma_ml = normalEstimates[1];
  estimates.normal_sigma_mml = normalEstimates[2];
  estimates.normal_msglen = message.encodeUsingNormalModel();
  /*cout << "\tMean: " << estimates.normal_mean << endl;
  cout << "\tSigma: " << estimates.normal_sigma << endl;
  cout << "\tMessage length: " << estimates.normal_msglen << endl;
  cout << endl;*/

  // Laplace estimates
  //cout << "*** LAPLACE ***" << endl;
  vector<double> laplaceEstimates = message.getLaplaceEstimates();
  estimates.laplace_mean = laplaceEstimates[0];
  estimates.laplace_scale_ml = laplaceEstimates[1];
  estimates.laplace_scale_mml = laplaceEstimates[2];
  estimates.laplace_msglen = message.encodeUsingLaplaceModel();
  /*cout << "\tMean: " << estimates.laplace_mean << endl;
  cout << "\tScale: " << estimates.laplace_scale << endl;
  cout << "\tMessage length: " << estimates.laplace_msglen << endl;
  cout << endl;*/

  return estimates;
}

