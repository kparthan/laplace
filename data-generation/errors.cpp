#include "Header.h"

int main(int argc, char *argv[])
{
  ifstream file("test/stackloss");
  string line;
  vector<double> numbers;
  double stackloss,ra,temp;
  double estimate,residual;
  double lsq_error,lad_error; 

  ofstream lsq("test/stackloss_lsq");
  ofstream lad("test/stackloss_lad");
  ofstream lad_pos("test/stackloss_lad_positive");

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
    stackloss = numbers[0] * 10;
    ra = numbers[1];
    temp = numbers[2];
    //cout << stackloss << " " << ra << " " << temp << endl;

    // least squares fit
    // stackloss = -3.466 + 0.0744 * RA + 0.1427 * TEMP
    estimate = -3.466 + 0.0744 * ra + 0.1427 * temp;
    residual = stackloss - estimate;
    cout << estimate << " " << residual << " ";
    lsq_error = residual * residual;
    lsq << fixed << setw(10) << setprecision(4) << lsq_error << endl;
    
    // lad fit
    // stackloss = -3.465 + 0.1085 * RA + 0.0456 * TEMP
    estimate = -3.465 + 0.1085 * ra + 0.0456 * temp;
    residual = stackloss - estimate;
    cout << estimate << " " << residual << endl;
    lad_error = fabs(residual);
    lad << fixed << setw(10) << setprecision(4) << residual << endl;
    lad_pos << fixed << setw(10) << setprecision(4) << lad_error << endl;

    numbers.clear();
  }

  lsq.close();
  lad.close();
  lad_pos.close();
  file.close();

  return 0;
}

