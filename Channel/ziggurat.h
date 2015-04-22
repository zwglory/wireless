//===========================================================================================
//	The Ziggurat method for generating random numbers is a special case of the rejection method, 
//	where the density of the proposal distribution looks like the silhouette of a Ziggurat.
//
//	The method can be used to produce very fast random number generators for distributions 
//	like the normal distribution(RNOR) or the exponential distribution(REXP). 
//
//	The main advantage comes from the fact that for a high percentage of the generated numbers 
//	no slow floating point operations are necessary.
//
//	The technical details of the Ziggurat method are, for example, contained in the article 
//	"The Ziggurat Method for Generating Random Variables" by G. Marsaglia and W. W. Tsang.

//	Author: baobao7766
//	Time:   2015-04-22
//===========================================================================================
#ifndef ZIGGURAT_H
#define ZIGGURAT_H

#include <math.h>

#define SHR3 (jz=jsr, jsr^=(jsr<<13), jsr^=(jsr>>17), jsr^=(jsr<<5), jz+jsr)	//shift right 3
#define UNI  (.5 + (signed)SHR3 * .2328306e-9)									//uniform
#define RNOR (hz = SHR3, iz = hz&127, (unsigned long(abs(hz))<kn[iz]) ? hz*wn[iz] : nfix())	//random normal
#define REXP (jz = SHR3, iz = jz&255, (jz<ke[iz]) ? jz*we[iz] : efix())						//random exponential

static unsigned long iz, jz, jsr=123456789, kn[128], ke[256];
static long hz;
static double wn[128], fn[128], we[256], fe[256];

double nfix(void){ /*provides RNOR if #define cannot*/
	const double r = 3.442620f;
	static double x, y;
	for(;;){
		x = hz * wn[iz];
		if(iz==0){
			do{
				x = -log(UNI)*0.2904764;
				y = -log(UNI);
			}
			while(y+y < x*x);
			return(hz>0)?r+x:-r-x;
		}
		if(fn[iz] + UNI*(fn[iz-1]-fn[iz]) < exp(-.5*x*x)) return x;
		hz = SHR3; 
		iz = hz&127;
		if(unsigned long(abs(hz)) < kn[iz]) return(hz*wn[iz]);
	} 
}// end nfix()

double efix(void) { /*provides REXP if #define cannot*/
	double x;
	for(;;){
		if(iz==0)	
			return(7.69711-log(UNI));
		x=jz * we[iz];
		if(fe[iz]+UNI*(fe[iz-1]-fe[iz]) < exp(-x))	
			return(x);
		jz=SHR3;
		iz=(jz&255);
		if(jz<ke[iz])	
			return(jz*we[iz]);
	} 
}// end efix()

/*--------This procedure sets the seed and creates the tables------*/
void zigset(unsigned long jsrseed) {
	const double 
		m1 = 2147483648.0, 
		m2 = 4294967296.;
	double 
		dn = 3.442619855899, 
		tn = dn, 
		vn = 9.91256303526217e-3, 
		q;
	double 
		de=7.697117470131487,
		te=de,
		ve=3.949659822581572e-3;
	int i; 
	jsr = jsrseed;
	/*Tables for NOR:*/ 
	q = vn/exp(-.5*dn*dn);
	kn[0] = unsigned long((dn/q)*m1);	kn[1] = 0;
	wn[0] = q/m1;		wn[127] = dn/m1;
	fn[0]=1.;			fn[127] = exp(-.5*dn*dn);
	for(i=126;i>=1;i--) {
		dn = sqrt(-2.*log(vn/dn+exp(-.5*dn*dn)));
		kn[i+1] = unsigned long((dn/tn)*m1); 
		tn = dn;
		fn[i] = exp(-.5*dn*dn);
		wn[i] = dn/m1;
	}
	/*Tables for REXP*/ 
	q = ve/exp(-de);
	ke[0] = unsigned long((de/q)*m2);	ke[1] = 0;
	we[0] = q/m2;		we[255]=de/m2;
	fe[0]=1.;			fe[255]=exp(-de);
	for(i=254;i>=1;i--) {
		de = -log(ve/de+exp(-de));
		ke[i+1] = unsigned long((de/te)*m2);
		te = de;
		fe[i] = exp(-de); 
		we[i] = de/m2; 
	} 
}// end zigset()

#endif