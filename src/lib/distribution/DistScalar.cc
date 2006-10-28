#include <config.h>
#include <distribution/DistScalar.h>
#include <sarray/SArray.h>
#include <sarray/util.h>
#include "Bounds.h"

#include <stdexcept>
#include <cfloat>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;
using std::string;
using std::vector;
using std::length_error;
using std::logic_error;

DistScalar::DistScalar(string const &name, unsigned int npar, Support support, 
		       bool canbound, bool discrete)
  
  : Distribution(name, npar, canbound, discrete), 
    _support(support)
{
}

void DistScalar::support(double *lower, double *upper, unsigned int length, 
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const &dims) 
    const
{
    support(lower, upper, parameters);
}

void DistScalar::support(double *lower, double *upper,
			 vector<double const *> const &parameters) const
{
    double const *lb = lowerBound(this, parameters);
    if (lb) {
	*lower = max(l(parameters), *lb);
    }
    else {
	*lower = l(parameters);
    }
    double const *ub = upperBound(this, parameters);
    if (ub) {
	*upper = min(u(parameters), *ub);
    }
    else {
	*upper = u(parameters);
    }

}

double DistScalar::l(vector<double const *> const &parameters) const
{
    switch(_support) {
    case DIST_UNBOUNDED:
	return -DBL_MAX;
	break;
    case DIST_POSITIVE: case DIST_PROPORTION:
	return 0;
	break;
    case DIST_SPECIAL:
	//You must overload this function 
	throw logic_error("Cannot call DistScalar::l for special distribution");
    }
    return 0; //Wall
}

double DistScalar::u(vector<double const *> const &parameters) const
{
    switch(_support) {
    case DIST_UNBOUNDED: case DIST_POSITIVE:
	return DBL_MAX;
	break;
    case DIST_PROPORTION:
	return 1;
	break;
    case DIST_SPECIAL:
	//You must overload this function 
	throw logic_error("Cannot call DistScalar::u for special distribution");
    }
    return 0; //Wall
}

bool DistScalar::isSupportFixed(vector<bool> const &fixmask) const
{
    if (_support == DIST_SPECIAL) {
	//You must overload this function 
	throw logic_error("Cannot call DistScalar::isSupportFixed for special distribution");
    }
    else {
	return true;
    }
}

double 
DistScalar::logLikelihood(double const *x, unsigned int length,
			  std::vector<double const *> const &parameters,
			  std::vector<std::vector<unsigned int> > const  &dims)
    const
{
    return logLikelihood(*x, parameters);
}

void 
DistScalar::randomSample(double *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const  &dims,
			 RNG *r) const
{
    *x = randomSample(parameters, r);
}

void 
DistScalar::typicalValue(double *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const &dims) const
{
    *x = typicalValue(parameters);
}

bool 
DistScalar::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
    return count_if(dims.begin(), dims.end(), isScalar) == dims.size();
}

