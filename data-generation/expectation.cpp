#include "Header.h"

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  vector<double> numbers;

  while(getline(file,line)) {
    boost::char_separator<char> sep(", ");
    boost::tokenizer<boost::char_separator<char> > tokens(line,sep);
    BOOST_FOREACH (const string& t, tokens) {
      istringstream iss(t);
      double x;
      iss >> x;
      numbers.push_back(x);
    }
  }
  file.close();

  double mean = 0;
  for (int i=0; i<numbers.size(); i++) {
    mean += numbers[i];
  }
  mean /= (double)numbers.size();
  double variance = 0;
  for (int i=0; i<numbers.size(); i++) {
    variance += (numbers[i] - mean) * (numbers[i] - mean);
  }
  variance /= (double) numbers.size();

  cout << "Size: " << numbers.size() << endl;
  cout << "Mean: " << mean << endl;
  cout << "Variance: " << variance << endl;

  return 0;
}

