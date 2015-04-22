
#include "WirelessType.h"

/* Power of signals. */
vec inv_dB(const vec &indB);
vec get_N0(const vec &EbN0, const double &Eb);

/* Sample points for signals. */
vec linspace(double from, double to, int points);

/* Print vector */
void print_vector(const bvec &in);
void print_vector(const vec &in);

/* Save vector to file. */
void save_vector(const bvec &in, std::string str);
void save_vector(const vec &in, std::string str);