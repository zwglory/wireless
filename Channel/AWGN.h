#ifndef AWGN_H
#define AWGN_H

#include <vector>
#include <math.h>
//#include "..\base\WirelessType.h"

using namespace std;

class AWGN{
public:
	//! Constructor. Set mean and variance.
	AWGN(double meanval, double variance):
		mean(meanval), sigma(sqrt(variance)), seed(2147483648) {}
	AWGN(double variance):
		mean(0.0), sigma(sqrt(variance)), seed(2147483648) {}
	AWGN(): mean(0.0), sigma(1.0), seed(2147483648) {}

	//! Destruction.
	~AWGN();

	//! Set mean, and variance
	void setup(double variance)
		{ mean = 0.0; sigma = sqrt(variance); }
	void setup(double meanval, double variance)
		{ mean = meanval; sigma = sqrt(variance); }

	//! Add gauss white noise
	vector<double> channel(vector<int> symbol);
	vector<double> channel(vector<double> symbol);
private:
	double mean, sigma;
	unsigned long seed;
};

#endif