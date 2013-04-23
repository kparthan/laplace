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
 *  \brief This function adds noise to the generated fucntion values
 *  \param sigma a double
 */
void DataGenerator::addNoise(double sigma)
{
  yvals = vector<double>(fxvals);
  NormalDataGenerator noise_generator(0,sigma);
  vector<double> noise = noise_generator.generateRandom(yvals.size());
  for(int i=0; i<yvals.size(); i++) {
    yvals[i] += noise[i];
  }
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

/*!
 *  \brief This function computes the MML estimates.
 *//*
void DataGenerator::mmlEstimate()
{
  Message message(x);
  message.estimateParameters();

  // Normal estimates
  cout << "*** NORMAL ***" << endl;
  pair<double,double> normalEstimates = message.getNormalEstimates();
  cout << "\tMean: " << normalEstimates.first << endl;
  cout << "\tSigma: " << normalEstimates.second << endl;
  cout << "\tMessage length: " << message.encodeUsingNormalModel() << endl;

  cout << endl;
  // Laplace estimates
  cout << "*** LAPLACE ***" << endl;
  pair<double,double> laplaceEstimates = message.getLaplaceEstimates();
  cout << "\tMean: " << laplaceEstimates.first << endl;
  cout << "\tScale: " << laplaceEstimates.second << endl;
  cout << "\tMessage length: " << message.encodeUsingLaplaceModel() << endl;
}
*/
