#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <cstdlib>
#include <iomanip>
#include <sys/stat.h>

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::program_options;
using namespace boost::filesystem;

typedef std::vector<double> Vector;

std::vector<Vector> load_table(string &file_name, int D)
{
  std::vector<Vector> sample;
  ifstream file(file_name.c_str());
  string line;
  Vector numbers(D,0);
  int i;
  while (getline(file,line)) {
    boost::char_separator<char> sep(" \t");
    boost::tokenizer<boost::char_separator<char> > tokens(line,sep);
    i = 0;
    BOOST_FOREACH(const string &t, tokens) {
      istringstream iss(t);
      double x;
      iss >> x;
      numbers[i++] = x;
    }
    sample.push_back(numbers);
  }
  file.close();
  return sample;
}

int partition(Vector &list, std::vector<int> &index, int left, int right)
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

void quicksort(Vector &list, std::vector<int> &index, int left, int right)
{
	if(left < right)
	{
		int pivotNewIndex = partition(list,index,left,right);
		quicksort(list,index,left,pivotNewIndex-1);
		quicksort(list,index,pivotNewIndex+1,right);
	}
}

Vector sort(Vector &list)
{
  int num_samples = list.size();
	Vector sortedList(list);
  std::vector<int> index(num_samples,0);
	for(int i=0; i<num_samples; i++) {
			index[i] = i;
  }
	quicksort(sortedList,index,0,num_samples-1);
  return sortedList;
}

std::vector<Vector> flip(std::vector<Vector> &table)
{
  int num_rows = table.size();
  Vector empty_vector(num_rows,0);
  int num_cols = table[0].size();
  std::vector<Vector> inverted_table(num_cols,empty_vector);
  for (int i=0; i<num_cols; i++) {
    for (int j=0; j<num_rows; j++) {
      inverted_table[i][j] = table[j][i];
    }
  }
  return inverted_table;
}

int minimumIndex(Vector &values)
{
  int min_index = 0;
  double min_val = values[0];
  for (int i=1; i<values.size(); i++) { 
    if (values[i] <= min_val) {
      min_index = i;
      min_val = values[i];
    } // if()
  } // for()
  return min_index;
}

int maximumIndex(Vector &values)
{
  int max_index = 0;
  double max_val = values[0];
  for (int i=1; i<values.size(); i++) { 
    if (values[i] > max_val) {
      max_index = i;
      max_val = values[i];
    }
  }
  return max_index;
}

double computeMedian(Vector &list)
{
  Vector sorted_list = sort(list);
  int n = sorted_list.size();
  if (n % 2 == 1) {
    return sorted_list[n/2];
  } else {
    return (sorted_list[n/2-1]+sorted_list[n/2])/2;
  }
}

Vector computeMedians(std::vector<Vector> &table)
{
  std::vector<Vector> inverted_table = flip(table);
  int num_cols = table[0].size();
  Vector medians(num_cols,0);
  for (int i=0; i<num_cols; i++) {
    medians[i] = computeMedian(inverted_table[i]);
  }
  return medians;
}

double computeMean(Vector &list)
{
  double sum = 0;
  for (int i=0; i<list.size(); i++) {
    sum += list[i];
  }
  return sum / (double)list.size();
}

double computeBiasSquared(Vector &values, double true_value)
{
  double avg_est = computeMean(values);
  double bias = avg_est - true_value;
  return bias * bias;
}

double computeMeanAbsoluteError(Vector &values, double true_value)
{
  Vector relative_errors(values.size(),0);
  for (int i=0; i<values.size(); i++) {
    relative_errors[i] = fabs(values[i] - true_value);
  }
  double avg_error = computeMean(relative_errors);
  return avg_error / true_value;
}

double computeMeanSquaredError(Vector &values, double true_value)
{
  Vector squared_errors(values.size(),0);
  for (int i=0; i<squared_errors.size(); i++) {
    double error = values[i] - true_value;
    squared_errors[i] = error * error;
  }
  double mse = computeMean(squared_errors);
  return mse;
}

int main(int argc, char **argv)
{
  // medians
  string file_name = "data/laplace_ml_scale_1_iter_100";
  std::vector<Vector> values = load_table(file_name,10);
  Vector medians_ml = computeMedians(values);
  string output_file = "medians_ml";
  ofstream out1(output_file.c_str());
  for (int i=0; i<medians_ml.size(); i++) {
    out1 << medians_ml[i] << endl;
  }
  out1.close();

  file_name = "data/laplace_mml_scale_1_iter_100";
  values = load_table(file_name,10);
  Vector medians_mml = computeMedians(values);
  output_file = "medians_mml";
  ofstream out2(output_file.c_str());
  for (int i=0; i<medians_mml.size(); i++) {
    out2 << medians_mml[i] << endl;
  }
  out2.close();

  // errors in estimation 
  double true_value = 1;
  file_name = "data/laplace_ml_scale_1_iter_100";
  values = load_table(file_name,10);
  std::vector<Vector> inverted_table = flip(values);
  output_file = "errors_ml";
  ofstream out3(output_file.c_str());
  for (int i=0; i<inverted_table.size(); i++) {
    double biassq = computeBiasSquared(inverted_table[i],true_value);
    out3 << fixed << scientific << setprecision(3) << biassq << "\t\t";
    double avg_rel_error = computeMeanAbsoluteError(inverted_table[i],true_value);
    out3 << fixed << scientific << setprecision(3) << avg_rel_error << "\t\t";
    double mse = computeMeanSquaredError(inverted_table[i],true_value);
    out3 << fixed << scientific << setprecision(3) << mse << endl;
  }
  out3.close();
  
  file_name = "data/laplace_mml_scale_1_iter_100";
  values = load_table(file_name,10);
  inverted_table = flip(values);
  output_file = "errors_mml";
  ofstream out4(output_file.c_str());
  for (int i=0; i<inverted_table.size(); i++) {
    double biassq = computeBiasSquared(inverted_table[i],true_value);
    out4 << fixed << scientific << setprecision(3) << biassq << "\t\t";
    double avg_rel_error = computeMeanAbsoluteError(inverted_table[i],true_value);
    out4 << fixed << scientific << setprecision(3) << avg_rel_error << "\t\t";
    double mse = computeMeanSquaredError(inverted_table[i],true_value);
    out4 << fixed << scientific << setprecision(3) << mse << endl;
  }
  out4.close();
}

