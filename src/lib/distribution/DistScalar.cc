#include <config.h>
#include <distribution/DistScalar.h>
#include <util/nainf.h>
#include <util/dim.h>

#include <stdexcept>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;
using std::string;
using std::vector;
using std::length_error;
using std::logic_error;
using std::count_if;

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
    *lower = l(parameters);
    *upper = u(parameters);
}

double DistScalar::l(vector<double const *> const &parameters) const
{
    switch(_support) {
    case DIST_UNBOUNDED:
	return JAGS_NEGINF;
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
	return JAGS_POSINF;
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
			  vector<double const *> const &parameters,
			  vector<vector<unsigned int> > const &dims,
                          double const *lower, double const *upper)
    const
{
    return logLikelihood(*x, parameters, lower, upper);
}

void 
DistScalar::randomSample(double *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const  &dims,
                         double const *lower, double const *upper,
			 RNG *r) const
{
    *x = randomSample(parameters, lower, upper, r);
}

void 
DistScalar::typicalValue (double *x, unsigned int length,
			  std::vector<double const *> const &parameters,
			  std::vector<std::vector<unsigned int> > const &dims,
                          double const *lower, double const *upper) const
{
    *x = typicalValue(parameters, lower, upper);
}

bool 
DistScalar::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
#ifdef _RWSTD_NO_CLASS_PARTIAL_SPEC
//Workaround for Solaris libCstd
    int n = 0; 
    count_if(dims.begin(), dims.end(), isScalar, n);
    return n == dims.size();
#else
    return count_if(dims.begin(), dims.end(), isScalar);
#endif
}

