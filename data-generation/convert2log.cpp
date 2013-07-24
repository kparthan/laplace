#include "Header.h"

int main(int argc, char *argv[])
{
  ifstream file("test/ball_bearings");
  string line;
  vector<double> numbers;

  int p = atoi(argv[1]);
  //cout << "Precision: " << p << endl;

  while(getline(file,line)) {
    boost::char_separator<char> sep(", ");
    boost::tokenizer<boost::char_separator<char> > tokens(line,sep);
    // get a row from the dataset
    BOOST_FOREACH (const string& t, tokens) {
      istringstream iss(t);
      double x;
      iss >> x;
      numbers.push_back(x);
    }
  }

  ofstream log_file("test/ball_bearings_log");
  for (int i=0; i<numbers.size(); i++) {
    //cout << numbers[i] << " " << log(numbers[i]) << endl;
    log_file << fixed << setw(10) << setprecision(p) << log(numbers[i]) << endl;
  }
  log_file.close();
  file.close();

  return 0;
}

