#ifndef FUNC_TEST_H_
#define FUNC_TEST_H_

#include "ScalarFunction.h"
#include "VectorFunction.h"

/*
  Function in JAGS are set up to take vectors of pointers as
  arguments, with additional arguments for lengths or dimensions
  of the arguments when necessary. This is not a good choice for the
  testing framework.
  
  These are wrappers to the members of Function and its subclasses
  that take care of setting up the arguments in the right way. We
  use these to simplify the testing framework.
*/

/* All functions */

//Check that function f accepts exactly n arguments.  We check that it
//does not accept any other value from 0 to 4n.
bool checknpar(jags::Function const *f, unsigned int n);

//Check whether function produces a discrete value
bool isdiscrete(jags::Function const *f, bool mask1);
bool isdiscrete(jags::Function const *f, bool mask1, bool mask2);
bool isdiscrete(jags::Function const *f, bool mask1, bool mask2, bool mask3);

//Check all possible values of mask and use a predicate
bool isdiscrete(jags::Function const *f, unsigned int npar,
		bool (*predicate) (std::vector<bool> const &));

//suitable predicates for isdiscrete
bool always(std::vector<bool> const &);
bool never(std::vector<bool> const &);
bool all(std::vector<bool> const &);
bool any(std::vector<bool> const &);

//Returns true if f is never a scale function
bool neverscale(jags::Function const *f, unsigned int npar);
//Returns true if f is never a linear function
bool neverlinear(jags::Function const *f, unsigned int npar);
//Returns true if f is never a power function
bool neverpow(jags::Function const *f, unsigned int npar);
//Returns true if f is never a linear, scale, or power function
bool neverslp(jags::Function const *f, unsigned int npar);

/* Scalar functions */

//Check that the limits of a scalar function are valid
void checkLimits(jags::ScalarFunction const *f, double lower, double upper);

//Single argument
double eval(jags::ScalarFunction const *f, double x);
bool checkval(jags::ScalarFunction const *f, double x);

//Two arguments
double eval(jags::ScalarFunction const *f, double x, double y);
bool checkval(jags::ScalarFunction const *f, double x, double y);

//Three arguments
double eval(jags::ScalarFunction const *f, double x, double y, double z);
bool checkval(jags::ScalarFunction const *f, double x, double y, double z);

/* Vector functions */

//Construct an STL vector from an array and the array length
std::vector<double> mkVec(double const *x, unsigned int N);

//Single argument
std::vector<double> veval(jags::VectorFunction const *f, 
			  std::vector<double> const &x);
bool checkval(jags::VectorFunction const *f, std::vector<double> const &x);
bool checkparlen(jags::VectorFunction const *f, unsigned int n);

//Two arguments
std::vector<double> veval(jags::VectorFunction const *f, 
			  std::vector<double> const &x, 
			  std::vector<double> const &y);
bool checkval(jags::VectorFunction const *f, std::vector<double> const &x,
	      std::vector<double> const &y);
bool checkparlen(jags::VectorFunction const *f, unsigned int n1, 
		 unsigned int n2);

/* Vector functions returning a scalar */

//Single argument
double eval(jags::VectorFunction const *f, std::vector<double> const &x);

//Two arguments
double eval(jags::VectorFunction const *f, std::vector<double> const &x, 
	    std::vector<double> const &y);
double eval(jags::VectorFunction const *f, double x, 
	    std::vector<double> const &y);
double eval(jags::VectorFunction const *f, std::vector<double> const &x, 
	    double y);

#endif /* FUNC_TEST_H_ */
