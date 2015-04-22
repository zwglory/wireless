
#include "AWGN.h"
#include "ziggurat.h"

using namespace std;

AWGN::~AWGN(){
	
}
/*********************************************************
	Add gauss noise over the transmitted symbol, 
	recive signal = self.channel(transmited symbol) 
**********************************************************/
vector<double> AWGN::channel(vector<int> symbol){
	vector<double> recive(symbol.size());
	vector<int>::iterator it_symbol = symbol.begin();
	vector<double>::iterator it_recive = recive.begin();
	zigset(seed);
	while(it_recive != recive.end()){
		*it_recive = *it_symbol + mean + sigma*RNOR;
		++it_recive;
		++it_symbol;
	}
	return recive;
}

vector<double> AWGN::channel(vector<double> symbol){
	vector<double> recive(symbol.size());
	vector<double>::iterator it_symbol = symbol.begin();
	vector<double>::iterator it_recive = recive.begin();
	zigset(seed);
	while(it_recive != recive.end()){
		*it_recive = *it_symbol + mean + sigma*RNOR;
		++it_recive;
		++it_symbol;
	}
	return recive;
}