#include <config.h>
#include <util/nainf.h>
#include "JRmath.h"

#include "DRound.h"

#include <cfloat>
#include <algorithm>

using std::min;
using std::max;
using std::vector;

#define T(par) (*par[0])
#define NDIGITS(par) (*par[1])

using std::vector;

static inline double value(vector<double const *> const &par)
{
    return fprec(T(par), NDIGITS(par));
}

namespace bugs {

    DRound::DRound()
	: ScalarDist("dround", 2, DIST_SPECIAL)
    {
    }

    bool 
    DRound::checkParameterValue(vector<double const *> const &par) const
    {
	return NDIGITS(par) >= 0;
    }
    
    double 
    DRound::logDensity(double y, PDFType type,
		       vector<double const *> const &par,
		       double const *lower, double const *upper) const
    {
	return (y == value(par)) ? 0 : JAGS_NEGINF;
}

    double DRound::randomSample(vector<double const *> const &par,
				double const *lower, double const *upper,
				RNG *rng) const
    {
	/* 
	   The random sample from DRound is not random at all,
	   but deterministic.
	*/
	
	return value(par);
    }
    
    double DRound::typicalValue(vector<double const *> const &par,
				double const *lower, double const *upper) const
    {
	return value(par);
    }
    
    unsigned int DRound::df(vector<unsigned int> const &lengths) const
    {
	return 0;

    }

    double DRound::l(vector<double const *> const &par) const
    {
	return value(par);
    }

    double DRound::u(vector<double const *> const &par) const
    {
	return value(par);
    }
    
    bool DRound::isSupportFixed(vector<bool> const &fixmask) const
    {
	return fixmask[0] && fixmask[1];
    }
    
    double DRound::KL(vector<double const *> const &par1,
		      vector<double const *> const &par2) const
    {
	if (value(par1) == value(par2)) {
	    return 0;
	}
	else {
	    return JAGS_POSINF;
	}
    }

}
