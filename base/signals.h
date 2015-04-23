#ifndef SIGNAL_H
#define SIGNAL_H

#include "..\WirelessType.h"

/* Power of signals. */
vec inv_dB(const vec &indB);
vec get_N0(const vec &EbN0, const double &Eb);

/* Sample points for signals. */
vec linspace(double from, double to, int points);

#endif