#include "NormalDataGenerator.h"
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
 *  \param scale_index an integer 
 *  \param noise_index an integer 
 *  \return the file name
 */
string DataGenerator::getFileName(const char *name,
                                  int scale_index, int noise_index)
{
  string file = name;
  double mean = parameters.mean;
  file += "_mean_" + boost::lexical_cast<string>(mean);
  double scale = parameters.scale[scale_index];
  file += "_scale_" + boost::lexical_cast<string>(scale);
  double noise_sigma = parameters.noise_sigma[noise_index];
  file += "_noise_" + boost::lexical_cast<string>(noise_sigma);
  file += ".txt";
  return file;
}

/*!
 *  \brief This function outputs the data to a file.
 *  \param file_name a reference to a string
 *  \param num_samples an integer
 *  \param estimates a reference to a struct MML_Estimates
 */
void DataGenerator::writeToFile(string &file_name, int num_samples,
                                struct MML_Estimates &estimates)
{
  string file = "estimates/data/" + file_name;
  ofstream fp(file.c_str(),ios::app);
  fp << num_samples << "\t";
  fp << estimates.normal_mml << "\t";
  fp << estimates.laplace_mml << endl;
  fp.close();
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
  string file = "predictions/data/" + file_name;
  ofstream fp(file.c_str());
  for (int i=0; i<x.size(); i++) {
    fp << x[i] << "\t" << fx[i] << "\t";
    fp << predictions[0][i] << "\t" << predictions[1][i] << endl;
  }
  fp.close();
}

/*!
 *  \brief This function plots the predictions for a given parameter set
 */
void DataGenerator::plotPredictions(string &data_file)
{
  Plot graph("predictions/script.p");
  pair<double,double> xrange,yrange;

  vector<string> labels(3,"");
  labels[0] = "";
  labels[1] = "x";
  labels[2] = "predictions";
  graph.label(labels);
  xrange = extremum(x);
  yrange = extremum(predictions[0]);
  graph.setRange(xrange,yrange);
  graph.sketch(data_file,predictions);
}

/*!
 *  \brief This function estimates the message length and generates the plots.
 *  \param name a pointer to a char 
 *  \param list a reference to a vector<double>
 *  \param scale_index an integer 
 *  \param noise_index an integer 
 */
void DataGenerator::estimateAndPlotModel(const char *name,
                                         vector<double> &list, 
                                         int scale_index, int noise_index)
{
  string file_name = getFileName(name,scale_index,noise_index);

  x = sort(list);
  fx = computeFunctionValues(x);
  struct MML_Estimates estimates = mmlEstimate(x);
  predictions = predict(x,estimates);
  writeToFile(file_name,x,fx,predictions);
  plotPredictions();

  addNoise(parameters.noise_sigma[noise_index]);
  fx_noise = computeFunctionValues(x_noise);
  struct MML_Estimates estimates_noise = mmlEstimate(x_noise);
  predictions_noise = predict(x_noise,estimates_noise);
  plotPredictions();

  writeToFile(file_name,list.size(),estimates);
}

/*!
 *  \brief This function adds noise to the generated fucntion values
 *  \param sigma a double
 */
void DataGenerator::addNoise(double sigma)
{
  x_noise = vector<double>(x);
  NormalDataGenerator noise_generator(0,sigma);
  vector<double> noise = noise_generator.generateRandom(x.size());
  for(int i=0; i<x.size(); i++) {
    x_noise[i] += noise[i];
  }
}

/*!
 *  \brief This function predicts the distribution with respect to the
 *  estimated parameters.
 *  \param x a reference to a vector<double>
 *  \param estimates a reference to a struct MML_Estimates
 *  \return the list of estimated values
 */
vector<vector<double>> DataGenerator::predict(vector<double> &x, 
                                      struct MML_Estimates &estimates)
{
  vector<vector<double>> y;

  // Estimate values using Normal distribution
  NormalDataGenerator normal(estimates.normal_mean,estimates.normal_sigma);
  vector<double> y0 = normal.computeFunctionValues(x);
  y.push_back(y0);

  // Estimate values using Laplace distribution
  LaplaceDataGenerator laplace(estimates.laplace_mean,estimates.laplace_scale);
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
struct MML_Estimates DataGenerator::mmlEstimate(vector<double> &x)
{
  Message message(x);
  message.estimateParameters();
  struct MML_Estimates estimates;

  // Normal estimates
  cout << "*** NORMAL ***" << endl;
  pair<double,double> normalEstimates = message.getNormalEstimates();
  estimates.normal_mean = normalEstimates.first;
  estimates.normal_sigma = normalEstimates.second;
  estimates.normal_mml = message.encodeUsingNormalModel();
  cout << "\tMean: " << estimates.normal_mean << endl;
  cout << "\tSigma: " << estimates.normal_sigma << endl;
  cout << "\tMessage length: " << estimates.normal_mml << endl;

  cout << endl;
  // Laplace estimates
  cout << "*** LAPLACE ***" << endl;
  pair<double,double> laplaceEstimates = message.getLaplaceEstimates();
  estimates.laplace_mean = laplaceEstimates.first;
  estimates.laplace_scale = laplaceEstimates.second;
  estimates.laplace_mml = message.encodeUsingLaplaceModel();
  cout << "\tMean: " << estimates.laplace_mean << endl;
  cout << "\tScale: " << estimates.laplace_scale << endl;
  cout << "\tMessage length: " << estimates.laplace_mml << endl;

  return estimates;
}

/*!
 *  \brief This function plots the random generated data
 *//*
void DataGenerator::plotData()
{
  pair<double,double> xrange,yrange;

  Plot graph1;
  vector<string> labels1(3,"");
  labels1[0] = "Laplace data plot";
  labels1[1] = "samples";
  labels1[2] = "x";
  graph1.label(labels1);
  xrange.first = 1;
  xrange.second = samples;
  yrange = extremum(x);
  graph1.setRange(xrange,yrange);
  graph1.sketch(x);

  Plot graph2;
  vector<string> labels2(3,"");
  labels2[0] = "Laplace data plot";
  labels2[1] = "x";
  labels2[2] = "f(x)";
  graph2.label(labels2);
  xrange = extremum(x);
  yrange = extremum(fx);
  graph2.setRange(xrange,yrange);
  graph2.sketch(x,fx);

  Plot graph3;
  vector<string> labels3(3,"");
  labels3[0] = "Laplace data plot";
  labels3[1] = "x";
  graph3.label(labels3);
  xrange = extremum(x);
  yrange = extremum(y);
  graph3.setRange(xrange,yrange);
  graph3.sketch(x,fx,y);
}*/

