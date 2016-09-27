#include <config.h>
#include <util/logical.h>
#include <util/nainf.h>
#include <util/dim.h>
#include <module/ModuleError.h>

#include "SumDist.h"

#include <cfloat>
#include <cmath>
#include <numeric>

using std::vector;
using std::fabs;
using std::sqrt;
using std::accumulate;

namespace jags {
namespace bugs {

    static const double TOL = sqrt(DBL_EPSILON);
    
    static double evaluate(vector <double const *> const &args,
			   vector<unsigned int> const &lengths)
    {
	double value = 0;
	for (unsigned int j = 0; j < args.size(); ++j) {
	    double const *a = args[j];
	    value = accumulate(a, a + lengths[j], value);
	}
	return value;
    }
    
    SumDist::SumDist()
	: VectorDist("sum", 0)
    {
    }
    
    bool SumDist::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }
    
    double SumDist::logDensity(double const *x, unsigned int length,
			       PDFType type,
			       vector<double const *> const &par,
			       vector<unsigned int> const &lengths,
			       double const *lower, double const *upper) const
    {
	return fabs(*x - evaluate(par, lengths)) > TOL ? JAGS_NEGINF : 0;
    }

    void SumDist::randomSample(double *x, unsigned int length,
			       vector<double const *> const &par, 
			       vector<unsigned int> const &lengths,
			       double const *lower, double const *upper,
			       RNG *rng) const
    {
	*x = evaluate(par, lengths);
    }

    bool SumDist::isSupportFixed(vector<bool> const &fixmask) const
    {
	return allTrue(fixmask);
    }

    unsigned int SumDist::df(vector<unsigned int> const &lengths) const
    {
	return 0;
    }

    bool SumDist::checkParameterValue(vector<double const *> const &params,
				      vector<unsigned int> const &lengths) const
    {
	return true;
    }

    bool
    SumDist::checkParameterLength (vector<unsigned int> const &lengths) const
    {
	if (lengths.empty()) return false;
	for (unsigned int i = 1; i < lengths.size(); ++i) {
	    if (lengths[i] == 0)
		return false;
	}
	return true;
    }

    bool SumDist::checkParameterDiscrete(vector<bool> const &mask) const
    {
	for (unsigned int i = 1; i < mask.size(); ++i) {
	    if (mask[i] != mask[0])
		return false;
	}
	return true;
    }

    void SumDist::support(double *lower, double *upper, unsigned int length,
			  vector<double const *> const &par,
			  vector<unsigned int> const &lengths) const
    {
	*lower = *upper = evaluate(par, lengths);
    }

    unsigned int SumDist::length(vector<unsigned int > const &lengths) const
    {
	return 1;
    }

}}
