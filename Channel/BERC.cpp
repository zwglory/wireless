#include <iostream>
#include <iomanip>		//std::setprecision(n), 设置有效数字为 n.
#include "BERC.h"

/***************************************************************
	The Bit error rate counter class (BERC)
***************************************************************/
/* Constructor. */
BERC::BERC(int indelay, int inignorefirst, int inignorelast):
    delay(indelay), ignorefirst(inignorefirst), ignorelast(inignorelast),
    errors(0), corrects(0) {}

/* Destruction */
BERC::~BERC(){}

/* Cumulative error counter */
void BERC::count(const bvec &in1, const bvec &in2)
{
	int countlength = std::min(in1.size(), in2.size()) - std::abs(delay)
					- ignorefirst - ignorelast;
	
	if (delay >= 0) {
		for (int i = 0; i < countlength; i++) {
			if (in1[i + ignorefirst] == in2[i + ignorefirst + delay]) {
				corrects++;
			}else {
				errors++;
			}//end if
		}//end for
	}else {
		for (int i = 0; i < countlength; i++) {
			if (in1[i + ignorefirst - delay] == in2[i + ignorefirst]) {
				corrects++;
			}else {
				errors++;
			}//end if
		}//end for
	}//end if
}

/* Run this member function if the delay between in1 and in2 is unknown. */
void BERC::estimate_delay(const bvec &in1, const bvec &in2, int mindelay,
                          int maxdelay)
{}

/* Writes an error report */
void BERC::report() const
{
	std::cout.setf(std::ios::fixed);
	std::cout << std::endl
			<< "==================================" << std::endl
			<< "     Bit Error Counter Report     " << std::endl
			<< "==================================" << std::endl
			<< " Ignore First           = " << ignorefirst << std::endl
			<< " Ignore Last            = " << ignorelast << std::endl
			<< " Delay                  = " << delay << std::endl
			<< " Number of counted bits = " << std::setprecision(0)
			<< (errors + corrects) << std::endl
			<< " Number of errors       = " << std::setprecision(0)
			<< errors << std::endl
			<< "==================================" << std::endl
			<< " Error rate             = " << std::setprecision(8)
			<< (errors / (errors + corrects)) << std::endl
			<< "==================================" << std::endl << std::endl;
}

/* Returns the number of errors between in1 and in2. */
double BERC::count_errors(const bvec &in1, const bvec &in2, int indelay,
                          int inignorefirst, int inignorelast)
{
	int countlength = std::min(in1.size(), in2.size()) - std::abs(indelay)
					- inignorefirst - inignorelast;
	int local_errors = 0;

	if (indelay >= 0) {
		for (int i = 0; i < countlength; i++) {
			if (in1[i + inignorefirst] != in2[i + inignorefirst + indelay]) {
				local_errors++;
			}//end if
		}//end for
	}else {
		for (int i = 0; i < countlength; i++) {
			if (in1[i + inignorefirst - indelay] != in2[i + inignorefirst]) {
				local_errors++;
			}//end if
		}//end for
	}//end if

	return local_errors;
}