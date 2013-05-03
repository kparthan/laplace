#include "Support.h"
#include "Message.h"

/*!
 *  \brief This is a null constructor used to instantiate the object
 */
Message::Message()
{}

/*!
 *  \brief This is a constructor function used to instantiate the object
 *  \param deviations a reference to a vector<array<double,3>>
 */
Message::Message(vector<array<double,3>> &deviations)
{
  /*vector<double> tmp;
  for (int i=0; i<3; i++){
    for (int j=0; j<deviations.size(); j++){
      tmp.push_back(deviations[j][i]);
    }
    samples.push_back(tmp);
    tmp.clear();
  }*/
  vector<double> tmp;
  for (int i=0; i<deviations.size(); i++) {
    for (int j=0; j<3; j++) {
      tmp.push_back(deviations[i][j]);
    }
  }
  samples.push_back(tmp);
}

/*!
 *  \brief This function computes the MML estimates of the parameters
 *  of a Normal distribution
 */
void Message::estimateNormalParameters()
{
  for (int i=0; i<1; i++) {
    array<double,2> estimates;
    double mean = computeMean(samples[i]); 
    //mean = 0;
    estimates[0] = mean;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += (samples[i][j] - mean) * (samples[i][j] - mean);
    }
    double sigma_square = sum / (double)(samples[i].size()-1);
    estimates[1] = sqrt(sigma_square);
    normal_estimates.push_back(estimates);
  }
}

/*!
 *  \brief This function sorts the elements in the list
 *  \param list a reference to a vector<double>
 *  \return the sorted list
 */
vector<double> Message::sort(vector<double> &list)
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
void Message::quicksort(vector<double> &list, vector<int> &index, 
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
int Message::partition(vector<double> &list, vector<int> &index,
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
 *  \brief This function computes the MML estimates of the parameters
 *  of a Laplace distribution
 */
void Message::estimateLaplaceParameters()
{
  for (int i=0; i<1; i++) {
    array<double,2> estimates;
    vector<double> list = sort(samples[i]);
    double median = computeMedian(list);
    //median = 0;
    estimates[0] = median;
    double sum = 0;
    for (int j=0; j<samples[i].size(); j++) {
      sum += fabs(samples[i][j]-median);
    }
    estimates[1] = sum / (double)(samples[i].size()-1);
    laplace_estimates.push_back(estimates);
  }
}

/*!
 *  \brief This function returns the Normal estimates of the parameters
 *  \return the Normal estimates
 */
vector<array<double,2>> Message::getNormalEstimates()
{
  estimateNormalParameters();
  return normal_estimates;
}

/*!
 *  \brief This function returns the Laplace estimates of the parameters
 *  \return the Laplace estimates
 */
vector<array<double,2>> Message::getLaplaceEstimates()
{
  estimateLaplaceParameters();
  return laplace_estimates;
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
vector<double> Message::encodeUsingNormalModel()
{
  vector<double> msglen(1,0);
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 5.0;
  double rangeLogSigma = 3.0;
  for (int i=0; i<1; i++) {
    int N = samples[i].size();
    double part1 = log(K2) + log(rangeMu * rangeLogSigma) + 0.5 * log(2 * N * N) 
                   - log(normal_estimates[i][1]);
    double part2 = (N/2.0) * log(2 * PI) - N * log(AOM) + 0.5 * (N+1) + 
                    + N * log(normal_estimates[i][1]);
    msglen[i] = (part1+part2)/log(2);
  }
  return msglen;
}

/*!
 *  \brief This module computes the encoded message length using
 *  Wallace Freeman formulation (for two parameters)
 *  \return the encoding length (in bits)
 */
vector<double> Message::encodeUsingLaplaceModel()
{
  vector<double> msglen(1,0);
  double K2 = 5.0 / (36 * sqrt(3));
  double rangeMu = 5.0;
  double rangeSigma = 3.0;
  for (int i=0; i<1; i++) {
    int N = samples[i].size();
    double part1 = log(K2) + log(rangeMu * rangeSigma) + log(N)
                   - log(laplace_estimates[i][1]);
    double part2 = N * log(2/AOM) + N * log(laplace_estimates[i][1]) + N;
    msglen[i] = (part1+part2)/log(2);
  }
  return msglen;
}

