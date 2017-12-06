/// Definition of correlator classes
#ifndef __correlator_h
#define __correlator_h

#include <stdio.h>

////////////////////////////////////////////////////
/// Standard Scalar Correlator f(tau)=<A(t)A(t+tau)>
class Correlator_Likh {

public:
	/** Where the coming values are stored */
	double **shift;
	/** Array containing the actual calculated correlation function */
	double **correlation;
	/** Number of values accumulated in cor */
	unsigned long int **ncorrelation;

	/** Accumulator in each correlator */
	double *accumulator;
	/** Index that controls accumulation in each correlator */
	unsigned int *naccumulator;
	/** Index pointing at the position at which the current value is inserted */
	unsigned int *insertindex;

	/** Number of Correlators */
	unsigned int numcorrelators;

	/** Minimum distance between points for correlators k>0; dmin = p/m */
	unsigned int dmin;

	/*  SCHEMATIC VIEW OF EACH CORRELATOR
						        p=N
		<----------------------------------------------->
		_________________________________________________
		|0|1|2|3|.|.|.| | | | | | | | | | | | | | | |N-1|
		-------------------------------------------------
		*/

	/** Lenght of result arrays */
	unsigned int length;
	/** Maximum correlator attained during simulation */
	unsigned int kmax;

// public:
	/** Points per correlator */
	unsigned int p;
	/** Number of points over which to average; RECOMMENDED: p mod m = 0 */
	unsigned int m;
	double *t, *f;
	unsigned int npcorr;

	/** Accumulated result of incoming values **/
	double accval;

	/** Constructor */
	Correlator_Likh() {numcorrelators=0;} ;
	Correlator_Likh (const unsigned int numcorrin, const unsigned int pin, const unsigned int min);
	~Correlator_Likh();

	/** Set size of correlator */
	void setsize (const unsigned int numcorrin = 32, const unsigned int pin = 16, const unsigned int min = 2);

	/** Add a scalar to the correlator number k */
	void add(const double w, const unsigned int k = 0);

	/** Evaluate the current state of the correlator */
	void evaluate(const bool norm = false);

	/** Initialize all values (current and average) to zero */
	void initialize();

	};

#endif
