#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "signals.h"

/* Power of signals */
vec inv_dB(const vec &indB){
	vec output(indB.size());
	vec::const_iterator it_indB = indB.begin();
	int i = 0;
	while(it_indB != indB.end()){
		output[i++] = pow(10, 0.1 * (*it_indB));
		++it_indB;
	}
	return output;
}

vec get_N0(const vec &EbN0, const double &Eb){
	vec output(EbN0.size());
	vec::const_iterator it_EbN0 = EbN0.begin();
	int i = 0;
	while(it_EbN0 != EbN0.end()){
		output[i++] = Eb * pow(*it_EbN0, -1.0);
		++it_EbN0;
	}
	return output;
}

/* Siganls */
vec linspace(double from, double to, int points)
{
  if (points < 2) {
    // This is the "Matlab definition" of linspace
    vec output(1);
    output[0] = to;
    return output;
  }else {
    vec output(points);
    double step = (to - from) / double(points - 1);
    int i;
    for (i = 0; i < points; i++)
      output[i] = from + i * step;
    return output;
  }
}

/* Print vector */
void print_vector(const bvec &in){
	bvec::const_iterator it = in.begin();
	while(it != in.end()){
		std::cout << *it << ' ';
		++it;
	}
	std::cout << std::endl;
}

void print_vector(const vec &in){
	vec::const_iterator it = in.begin();
	while(it != in.end()){
		std::cout << *it << ' ';
		++it;
	}
	std::cout << std::endl;
}

/* Save vector to file. */
void save_vector(const bvec &in, std::string str){
	std::string path("Data\\");
	std::string extension(".txt");
	std::ofstream fout(path + str + extension, std::ios::app);
	bvec::const_iterator it = in.begin();
	while(it != in.end()-1){
		fout << *it << ',';
		++it;
	}
	fout << *it << std::endl;
	if(fout){
		fout.clear();
		fout.close();
	}
	std::cout << "Writting file complete." << std::endl;
}

void save_vector(const vec &in, std::string str){
	std::string path("Results\\");
	std::string extension(".txt");
	std::ofstream fout(path + str + extension, std::ios::app);
	vec::const_iterator it = in.begin();
	while(it != in.end()-1){
		fout << *it << ',';
		++it;
	}
	fout << *it << std::endl;
	if(fout){
		fout.clear();
		fout.close();
	}
	std::cout << "Writting file complete." << std::endl;
}