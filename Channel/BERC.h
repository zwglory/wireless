#ifndef BERC_H
#define BERC_H

#include <vector>
#include <math.h>
#include "..\WirelessType.h"

class BERC{
public:
	//! Constructor. 
	BERC(int indelay = 0, int inignorefirst = 0, int inignorelast = 0);
	//! Destruction.
	~BERC();
	//! Cumulative error counter
	void count(const bvec &vec1, const bvec &vec2);
	//! Run this member function if the delay between \a in1 and
	//! \a in2 is unknown.
	void estimate_delay(const bvec &in1, const bvec &in2, int mindelay = -100,
						int maxdelay = 100);
	//! Clears the bit error counter
	void clear() { errors = 0; corrects = 0; }
	//! Writes an error report
	void report() const;
	//! Return the \a delay, assumed or estimated, between \a in1 and \a in2.
	int get_delay() const { return delay; }
	//! Returns the counted number of bit errors
	double get_errors() const { return errors; }
	//! Returns the counted number of corectly received bits
	double get_corrects() const { return corrects; }
	//! Returns the total number of bits processed
	double get_total_bits() const { return (errors + corrects); }
	//! Returns the estimated bit error rate.
	double get_errorrate() const { return (errors / (corrects + errors)); }

	/*!
	\brief static function to allow simple and fast count of bit-errors

	Returns the number of errors between in1 and in2. Typical usage:
	\code
	bvec in1 = randb(100);
	bvec in2 = randb(100);
	double errors = BERC::count_errors(in1, in2);
	\endcode
	*/
	static double count_errors(const bvec &in1, const bvec &in2,
								int indelay = 0, int inignorefirst = 0,
								int inignorelast = 0);

private:
	int delay;
	int ignorefirst;
	int ignorelast;
	double errors;
	double corrects;
};

#endif