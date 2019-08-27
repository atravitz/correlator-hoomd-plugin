/// Definition of correlator classes
#ifndef _CORRELATOR_H__
#define _CORRELATOR_H__

#include <stdio.h>

////////////////////////////////////////////////////
/// Standard Scalar Correlator f(tau)=<A(t)A(t+tau)>
class Correlator_Likh {
 public:
  double **shift;        // Where the incoming values are stored
  double **correlation;  // Array for the actual calculated correlation function
  unsigned long int **ncorrelation;  // Number of values accumulated in corr
  double *accumulator;               // Accumulator in each correlator
  unsigned int
      *naccumulator;  // Index that controls accumulation in each correlator
  unsigned int *insertindex;    // Index pointing at the position where current
                                // value is inserted
  unsigned int numcorrelators;  // Number of Correlators
  unsigned int d_min;  // Minimum distance between points for correlators k>0
                       // d_min = p/m

  /*  SCHEMATIC VIEW OF EACH CORRELATOR
                                            p=N
    <----------------------------------------------->
    _________________________________________________
    |0|1|2|3|.|.|.| | | | | | | | | | | | | | | |N-1|
    -------------------------------------------------
    */

  unsigned int length;  // Length of result arrays
  unsigned int kmax;    // Maximum correlator attained during simulation
  unsigned int p;       // Points per correlator
  unsigned int
      m;  // Number of points over which to average; RECOMMENDED: p mod m = 0
  double *t, *f;
  unsigned int npcorr;
  double accval;  // Accumulated result of incoming values

  // Constructor
  Correlator_Likh() { numcorrelators = 0; };
  Correlator_Likh(const unsigned int numcorrin, const unsigned int p_in,
                  const unsigned int m_in);
  ~Correlator_Likh();

  void setsize(const unsigned int numcorrin = 32,  // Set size of correlator
               const unsigned int p_in = 16, const unsigned int m_in = 2);

  void add(const double w,
           const unsigned int k = 0);  // Add an int to the corr number k
  void evaluate(
      const bool norm = false);  // Evaluate the current state of the correlator
  void initialize();  // Initialize all values (current and average) to zero
};

#endif
