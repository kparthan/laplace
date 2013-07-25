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
 *  \brief This function adjusts the data to AOM precision
 *  \param data a reference to a vector<double>
 *  \return the modified data values
 */
vector<double> DataGenerator::modifyDataToAOM(vector<double> &data, double aom)
{
  vector<double> x(data.size(),0);
  for (int i=0; i<data.size(); i++) {
    int val = data[i] / aom;
    x[i] = ((double)val) * aom;
  }
  return x;
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
  file += "_iter_" + boost::lexical_cast<string>(parameters.iterations);
  //cout << file << endl;
  return file;
}

/*!
 *  \brief This function prints the MML estimates for all the iterations. 
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

  // print normal estimates
  fp << setw(10) << setprecision(3) << estimates.normal_mean << "\t";
  fp << setw(10) << setprecision(3) << estimates.normal_scale_ml << "\t";
  fp << setw(10) << setprecision(3) << estimates.normal_scale_mml << "\t";
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
  /*double normal_mean_error = estimates.normal_mean - parameters.mean; 
  double normal_ml_sigma_error = estimates.normal_scale_ml - parameters.scale[scale_index];
  double normal_ml_sigma_error_sq = normal_ml_sigma_error * normal_ml_sigma_error;
  double normal_mml_sigma_error = estimates.normal_scale_mml - parameters.scale[scale_index];
  double normal_mml_sigma_error_sq = normal_mml_sigma_error * normal_mml_sigma_error;
  fp << setw(10) << setprecision(3) << normal_mean_error << "\t";
  fp << setw(10) << setprecision(3) << normal_ml_sigma_error << "\t";
  fp << setw(10) << setprecision(3) << normal_ml_sigma_error_sq << "\t";
  fp << fixed << setw(10) << setprecision(3) << normal_mml_sigma_error << "\t";
  fp << fixed << setw(10) << setprecision(3) << normal_mml_sigma_error_sq << "\t";

  // print difference in laplace estimates
  double laplace_mean_error = estimates.laplace_mean - parameters.mean; 
  double laplace_ml_scale_error = estimates.laplace_scale_ml - parameters.scale[scale_index];
  double laplace_ml_scale_error_sq = laplace_ml_scale_error * laplace_ml_scale_error;
  double laplace_mml_scale_error = estimates.laplace_scale_mml - parameters.scale[scale_index];
  double laplace_mml_scale_error_sq = laplace_mml_scale_error * laplace_mml_scale_error;
  fp << setw(10) << setprecision(3) << laplace_mean_error << "\t";
  fp << setw(10) << setprecision(3) << laplace_ml_scale_error << "\t";
  fp << setw(10) << setprecision(3) << laplace_ml_scale_error_sq << "\t";
  fp << fixed << setw(10) << setprecision(3) << laplace_mml_scale_error << "\t";
  fp << setw(10) << setprecision(3) << laplace_mml_scale_error_sq << "\t";*/

  // calculate the difference in msg len
  /*double diff_msglen;
  if (type == LAPLACE) {
    diff_msglen = estimates.normal_msglen - estimates.laplace_msglen;
  } else if (type == NORMAL) {
    diff_msglen = estimates.laplace_msglen - estimates.normal_msglen;
  }
  fp << fixed << setw(10) << setprecision(3) << diff_msglen << "\t";
  fp << endl;
  fp.close();*/
}

/*!
 *  \brief This function updates the statistics over all iterations
 *  \param n an integer
 *  \param estimates a reference to a struct Estimates
 *  \param statistics a reference to a struct Statistics
 */
/*void DataGenerator::updateStatistics(int n, int scale_index, struct Estimates &estimates,
                                     struct Statistics &statistics)
{
  double scale = parameters.scale[scale_index];
  double sum,residual,error;
  if (n == 1) {
    // normal
    statistics.normal_scale = vector<double>(8,0);
    statistics.normal_scale[0] = estimates.normal_scale_ml;
    statistics.normal_scale[1] = estimates.normal_scale_ml;
    statistics.normal_scale[2] = estimates.normal_scale_ml;
    residual = estimates.normal_scale_ml - scale;
    statistics.normal_scale[3] = residual * residual;
    statistics.normal_scale[4] = estimates.normal_scale_mml;
    statistics.normal_scale[5] = estimates.normal_scale_mml;
    statistics.normal_scale[6] = estimates.normal_scale_mml;
    residual = estimates.normal_scale_mml - scale;
    statistics.normal_scale[7] = residual * residual;
    // laplace
    statistics.laplace_scale = vector<double>(8,0);
    statistics.laplace_scale[0] = estimates.laplace_scale_ml;
    statistics.laplace_scale[1] = estimates.laplace_scale_ml;
    statistics.laplace_scale[2] = estimates.laplace_scale_ml;
    residual = estimates.laplace_scale_ml - scale;
    statistics.laplace_scale[3] = residual * residual;
    statistics.laplace_scale[4] = estimates.laplace_scale_mml;
    statistics.laplace_scale[5] = estimates.laplace_scale_mml;
    statistics.laplace_scale[6] = estimates.laplace_scale_mml;
    residual = estimates.laplace_scale_mml - scale;
    statistics.laplace_scale[7] = residual * residual;
  } else {
    // update normal statistics
    {
      // update ML average
      sum = (n-1) * statistics.normal_scale[0];
      statistics.normal_scale[0] = (sum + estimates.normal_scale_ml) / n;
      // update ML min
      if (statistics.normal_scale[1] > estimates.normal_scale_ml) {
        statistics.normal_scale[1] = estimates.normal_scale_ml;
      }
      // update ML max
      if (statistics.normal_scale[2] < estimates.normal_scale_ml) {
        statistics.normal_scale[2] = estimates.normal_scale_ml;
      }
      // update ML error
      sum = (n-1) * statistics.normal_scale[3];
      residual = estimates.normal_scale_ml - scale;
      error = residual * residual;
      statistics.normal_scale[3] = (sum+error)/n;
      // update MML average
      sum = (n-1) * statistics.normal_scale[4];
      statistics.normal_scale[4] = (sum + estimates.normal_scale_mml) / n;
      // update MML min
      if (statistics.normal_scale[5] > estimates.normal_scale_mml) {
        statistics.normal_scale[5] = estimates.normal_scale_mml;
      }
      // update MML max
      if (statistics.normal_scale[6] < estimates.normal_scale_mml) {
        statistics.normal_scale[6] = estimates.normal_scale_mml;
      }
      // update MML error
      sum = (n-1) * statistics.normal_scale[7];
      residual = estimates.normal_scale_mml - scale;
      error = residual * residual;
      statistics.normal_scale[7] = (sum+error)/n;
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
      // update ML error
      sum = (n-1) * statistics.laplace_scale[3];
      residual = estimates.laplace_scale_ml - scale;
      error = residual * residual;
      statistics.laplace_scale[3] = (sum+error)/n;
      // update MML average
      sum = (n-1) * statistics.laplace_scale[4];
      statistics.laplace_scale[4] = (sum + estimates.laplace_scale_mml) / n;
      // update MML min
      if (statistics.laplace_scale[5] > estimates.laplace_scale_mml) {
        statistics.laplace_scale[5] = estimates.laplace_scale_mml;
      }
      // update MML max
      if (statistics.laplace_scale[6] < estimates.laplace_scale_mml) {
        statistics.laplace_scale[6] = estimates.laplace_scale_mml;
      }
      // update MML error
      sum = (n-1) * statistics.laplace_scale[7];
      residual = estimates.laplace_scale_mml - scale;
      error = residual * residual;
      statistics.laplace_scale[7] = (sum+error)/n;
    }
  }
}*/

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
  xrange = pair<double,double>(-5,5);
  yrange = pair<double,double>(0,0.7);
  graph.setRange(xrange,yrange);
  graph.sketch(data_file,predictions_mml);
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
  struct Estimates estimates = parameterEstimation(x,parameters.aom);
  predict(x,estimates);

  vector<double> likelihood = logLikelihood(predictions_ml);
  estimates.normal_likelihood = likelihood[0];
  estimates.laplace_likelihood = likelihood[1];
  if (estimates.normal_likelihood > estimates.laplace_likelihood) {
    estimates.winner_ml = NORMAL;
  } else {
    estimates.winner_ml = LAPLACE;
  }

  string file_name = getFileName(name,list.size(),scale_index);
  if (parameters.iterations == 1) {
    printEstimates(estimates);
    writeToFile(file_name,x,fx,predictions_mml);
    plotPredictions(file_name);
  } else {
    updateResults(file_name,list.size(),scale_index,estimates);
  }

  predictions_ml[0].clear();predictions_ml[1].clear();predictions_ml.clear();
  predictions_mml[0].clear();predictions_mml[1].clear();predictions_mml.clear();
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
  xrange = pair<double,double>(1,101);
  yrange = pair<double,double>(5900,6400);
  graph.setRange(xrange,yrange);
  graph.sketchStatistics(file_name);
}

/*!
 *  \brief This function saves the statistics to the file
 *  \param statistics a reference to a struct Statistics
 *  \param iterations an integer
 *  \param sample_index an integer
 *  \param scale_index an integer
 */
/*void DataGenerator::saveErrorStatistics(const char *name, struct Statistics &statistics,
                                        int iterations, int sample_index,
                                        int scale_index)
{
  double scale = parameters.scale[scale_index];
  int num_samples = parameters.samples[sample_index];
  string file_name = "results/data/";
  file_name += "errors_" + string(name) + "_i_" + boost::lexical_cast<string>(iterations) + 
               "_s_" + boost::lexical_cast<string>(scale).substr(0,3);
  ofstream file(file_name.c_str(),ios::app);
  file << fixed << setw(10) << num_samples; 
  for (int i=0; i<statistics.normal_scale.size(); i++) {
    file << fixed << setw(10) << setprecision(5) << statistics.normal_scale[i];
  }
  for (int i=0; i<statistics.laplace_scale.size(); i++) {
    file << fixed << setw(10) << setprecision(5) << statistics.laplace_scale[i];
  }
  // print expectation values
  double expectation = scale * scale / (double) num_samples; 
  file << fixed << setw(10) << setprecision(5) << expectation;
  expectation = scale * scale * (num_samples+1) / (double) ((num_samples-1)*(num_samples-1));
  file << fixed << setw(10) << setprecision(5) << expectation;
  file << endl;
  file.close();
}*/

/*!
 *  \brief This method plots the errors in parameter estimates.
 *  \param name a pointer to a char
 */
/*void DataGenerator::plotErrors(const char *name)
{
  if (parameters.samples.size() > 1) {
    for (int i=0; i<parameters.scale.size(); i++) {
      double scale = parameters.scale[i];
      string data_file = "errors_" + string(name) + "_i_" + 
                         boost::lexical_cast<string>(parameters.iterations)
                         + "_s_" + boost::lexical_cast<string>(scale).substr(0,3);
      ofstream script("results/script.plot");
      script << "set term post eps" << endl;
      script << "set title \"Errors in Parameter Estimation\"" <<  endl;
      script << "set xlabel \"# of samples\"" << endl;
      script << "set ylabel \"Scale Parameter\"" << endl;
      script << "set xr [0:" << parameters.samples[parameters.samples.size()-1]+10 << "]" << endl;
      script << "set yr [0:3]" << endl;
      script << "set output \"results/plots/" << data_file << ".normal_est.eps\"" << endl;
      script << "set multiplot" << endl;
      script << "plot \"results/data/" << data_file << "\" using 1:2:3:4 title " 
             << "'ML estimate' with errorbars lc rgb \"blue\", \\" << endl;
      script << "\"results/data/" << data_file << "\" using 1:6:7:8 title " 
             << "'MML estimate' with errorbars lc rgb \"red\", \\" << endl;
      script << "\"results/data/" << data_file << "\" using 1:2 notitle " 
             << "with lines lc rgb \"blue\", \\" << endl;
      script << "\"results/data/" << data_file << "\" using 1:6 notitle " 
             << "with lines lc rgb \"red\"" << endl;
      script.close();
      system("gnuplot -persist results/script.plot");	

      ofstream script2("results/script.plot");
      script2 << "set term post eps" << endl;
      script2 << "set title \"Errors in Parameter Estimation\"" <<  endl;
      script2 << "set xlabel \"# of samples\"" << endl;
      script2 << "set ylabel \"Scale Parameter\"" << endl;
      script2 << "set xr [0:" << parameters.samples[parameters.samples.size()-1]+10 << "]" << endl;
      script2 << "set yr [0:3]" << endl;
      script2 << "set output \"results/plots/" << data_file << ".laplace_est.eps\"" << endl;
      script2 << "set multiplot" << endl;
      script2 << "plot \"results/data/" << data_file << "\" using 1:10:11:12 title " 
             << "'ML estimate' with errorbars lc rgb \"blue\", \\" << endl;
      script2 << "\"results/data/" << data_file << "\" using 1:14:15:16 title " 
             << "'MML estimate' with errorbars lc rgb \"red\", \\" << endl;
      script2 << "\"results/data/" << data_file << "\" using 1:10 notitle " 
             << "with lines lc rgb \"blue\", \\" << endl;
      script2 << "\"results/data/" << data_file << "\" using 1:14 notitle " 
             << "with lines lc rgb \"red\"" << endl;
      script2.close();
      system("gnuplot -persist results/script.plot");	
    }
  }
}*/

/*!
 *  \brief This function predicts the distribution with respect to the
 *  estimated parameters.
 *  \param x a reference to a vector<double>
 *  \param estimates a reference to a struct Estimates
 *  \return the list of estimated values
 */
void DataGenerator::predict(vector<double> &x, 
                                      struct Estimates &estimates)
{
  vector<double> y;

  // Estimate values using Normal distribution
  // ML
  NormalDataGenerator normal_ml(estimates.normal_mean,estimates.normal_scale_ml);
  y = normal_ml.computeFunctionValues(x);
  predictions_ml.push_back(y);
  // MML
  NormalDataGenerator normal_mml(estimates.normal_mean,estimates.normal_scale_mml);
  y = normal_mml.computeFunctionValues(x);
  predictions_mml.push_back(y);

  // Estimate values using Laplace distribution
  // ML
  LaplaceDataGenerator laplace_ml(estimates.laplace_mean,estimates.laplace_scale_ml);
  y = laplace_ml.computeFunctionValues(x);
  predictions_ml.push_back(y);
  // MML
  LaplaceDataGenerator laplace_mml(estimates.laplace_mean,estimates.laplace_scale_mml);
  y = laplace_mml.computeFunctionValues(x);
  predictions_mml.push_back(y);
}

/*!
 *  \brief This function computes the MML estimates.
 *  \param x a reference to a vector<double>
 *  \param aom a double
 *  \return the MML estimates of the parameters and the corresponding
 *  message length
 */
struct Estimates DataGenerator::parameterEstimation(vector<double> &x, double aom)
{
  Message message(x,aom);
  message.estimateParameters();
  struct Estimates estimates;

  // Normal estimates
  //cout << "*** NORMAL ***" << endl;
  vector<double> normalEstimates = message.getNormalEstimates();
  estimates.normal_mean = normalEstimates[0];
  estimates.normal_scale_ml = normalEstimates[1];
  estimates.normal_scale_mml = normalEstimates[2];
  estimates.normal_msglen = message.encodeUsingNormalModel();

  // Laplace estimates
  //cout << "*** LAPLACE ***" << endl;
  vector<double> laplaceEstimates = message.getLaplaceEstimates();
  estimates.laplace_mean = laplaceEstimates[0];
  estimates.laplace_scale_ml = laplaceEstimates[1];
  estimates.laplace_scale_mml = laplaceEstimates[2];
  estimates.laplace_msglen = message.encodeUsingLaplaceModel();

  if (estimates.normal_msglen < estimates.laplace_msglen) {
    estimates.winner_mml = NORMAL; // normal wins
  } else if (estimates.normal_msglen > estimates.laplace_msglen) {
    estimates.winner_mml = LAPLACE; // laplace wins
  } else {
    estimates.winner_mml = DEFAULT; // draw
  }

  return estimates;
}

/*!
 *
 */
vector<double> DataGenerator::logLikelihood(vector<vector<double>> &y)
{
  vector<double> likelihood(2,0);
  for (int i=0; i<y.size(); i++) {
    for (int j=0; j<y[i].size(); j++) {
      double value = y[i][j];
      likelihood[i] += log(value);
    }
  }
  return likelihood;
}

