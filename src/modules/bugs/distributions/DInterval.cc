#include <config.h>
#include "DInterval.h"
#include <util/dim.h>
#include <util/nainf.h>

#include <cfloat>
#include <algorithm>

using std::min;
using std::max;
using std::vector;

#define T(par) (*par[0])
#define CUTPOINTS(par) (par[1])
#define NCUT(lengths) (lengths[1])

static unsigned int value(vector<double const *> const &par, unsigned int ncut)
{
    double t = T(par);
    for (unsigned int i = 0; i < ncut; ++i) {
	if (t <= CUTPOINTS(par)[i])
	    return i;
    }
    return ncut;
}

namespace bugs {

DInterval::DInterval()
    : VectorDist("dinterval", 2)
{
}

bool DInterval::isDiscreteValued(vector<bool> const &mask) const
{
    return true;
}

bool DInterval::checkParameterLength(vector<unsigned int> const &lengths) const
{
    return lengths[0] == 1;
}

bool DInterval::checkParameterValue(vector<double const *> const &par,
				    vector<unsigned int> const &lengths) 
    const
{
    for (unsigned int i = 1; i < NCUT(lengths); ++i) {
	if (CUTPOINTS(par)[i] <= CUTPOINTS(par)[i-1])
	    return false;
    }
    return true;
}

double 
DInterval::logDensity(double const *y, unsigned int length, PDFType type,
		      vector<double const *> const &par,
		      vector<unsigned int> const &lengths,
		      double const *lower, double const *upper) const
{
    if (*y < 0)
	return JAGS_NEGINF;
    
    unsigned int x = static_cast<unsigned int>(*y);
    if (x > NCUT(lengths)) {
	return JAGS_NEGINF;
    }
    else {
	double t = T(par);
	if (x > 0 && t <= CUTPOINTS(par)[x-1])
	    return JAGS_NEGINF;
	else if (x < NCUT(lengths) && t > CUTPOINTS(par)[x])
	    return JAGS_NEGINF;
	else
	    return 0;
    }
}

void DInterval::randomSample(double  *x, unsigned int length,
			     vector<double const *> const &par,
			     vector<unsigned int> const &lengths,
			     double const *lower, double const *upper,
			     RNG *rng) const
{
    /* 
       The random sample from DInterval is not random at all,
       but deterministic.
    */
    *x = static_cast<double>(value(par, NCUT(lengths)));
}

void DInterval::typicalValue(double *x, unsigned int length,
			     vector<double const *> const &par,
			     vector<unsigned int> const &lengths,
			     double const *lower, double const *upper) const
{
    *x = static_cast<double>(value(par, NCUT(lengths)));
}

unsigned int DInterval::df(vector<unsigned int> const &lengths) const
{
    return 0;
}

void DInterval::support(double *lower, double *upper, unsigned int length,
			vector<double const *> const &par,
			vector<unsigned int> const &lengths) const
{
    unsigned int y = value(par, NCUT(lengths));    
    *lower = y;
    *upper = y;
}


bool DInterval::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1];
}

unsigned int DInterval::length(vector<unsigned int> const &params) const
{
    return 1;
}

double DInterval::KL(vector<double const *> const &par1,
		     vector<double const *> const &par2,
		     vector<unsigned int> const &lengths) const
{
    unsigned int y1 = value(par1, NCUT(lengths));
    unsigned int y2 = value(par2, NCUT(lengths));
    if (y1 == y2) {
	return 0;
    }
    else {
	return JAGS_POSINF;
    }
}

}
