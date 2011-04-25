#include <config.h>
#include "DCat.h"
#include <rng/RNG.h>
#include <util/dim.h>
#include <util/nainf.h>
#include <module/ModuleError.h>

#include <cfloat>
#include <algorithm>
#include <cmath>

#include <JRmath.h>

using std::min;
using std::max;
using std::vector;
using std::max_element;

#define PROB(par) (par[0])
#define NCAT(lengths) (lengths[0])

namespace bugs {

DCat::DCat()
    : VectorDist("dcat", 1) 
{}

bool DCat::isDiscreteValued(vector<bool> const &mask) const
{
    return true;
}

bool DCat::checkParameterValue(vector<double const *> const &par,
			       vector<unsigned int> const &lengths) const
{
    bool nz = false;
    for (unsigned int i = 0; i < NCAT(lengths); i++) {
	if (PROB(par)[i] < 0.0) {
	    return false; 
	}
	if (PROB(par)[i] > 0.0) {
	    nz = true; //at least one non-zero probability
	}
    }
    return nz;
}

double DCat::logDensity(double const *x, unsigned int length, PDFType type,
			vector<double const *> const &par,
			vector<unsigned int> const &lengths,
			double const *lower, double const *upper) const
{
    unsigned int y = static_cast<unsigned int>(*x);
    if (y < 1 || y > NCAT(lengths)) {
	return JAGS_NEGINF;
    }
    
    if (type == PDF_PRIOR) {
	//No need to calculate the normalizing constant
	return log(PROB(par)[y-1]);
    }
    else {
	//Need to normalize the log density
	double sump = 0.0;
	for (unsigned int i = 0; i < NCAT(lengths); i++) {
	    sump += PROB(par)[i];
	}
	return log(PROB(par)[y-1]) - log(sump);
    }
}

void DCat::randomSample(double *x, unsigned int length,
			vector<double const *> const &par,
			vector<unsigned int> const &lengths,
			double const *lower, double const *upper,
			RNG *rng) const
{
    double sump = 0;
    unsigned int i = 0;

    for ( ; i < NCAT(lengths); ++i) {
	sump += PROB(par)[i];
    }
    double p = sump * rng->uniform();    

    for ( ; i > 1; --i) {
	sump -= PROB(par)[i-1];
	if (sump <= p) 
            break;
    }
    *x  = i;
}

void DCat::support(double *lower, double *upper, unsigned int length,
	           vector<double const *> const &par,
	           vector<unsigned int> const &lengths) const
{
    if (length != 1)
	throwLogicError("Invalid length in DCat::support");

    *lower = 1;
    *upper = NCAT(lengths);
}

void DCat::typicalValue(double *x, unsigned int length,
			vector<double const *> const &par,
			vector<unsigned int> const &lengths,
			double const *lower, double const *upper) const
{
    *x = max_element(PROB(par), PROB(par) + NCAT(lengths)) - PROB(par) + 1;
}

bool DCat::isSupportFixed(vector<bool> const &fixmask) const
{
    return true;
}

bool DCat::checkParameterLength(vector<unsigned int> const &lengths) const
{
    return true;
}

unsigned int DCat::length(vector<unsigned int> const &lengths) const
{
    return 1;
}

double DCat::KL(vector<double const *> const &par1,
		vector<double const *> const &par2,
		vector<unsigned int> const &lengths) const
{
    double psum1 = 0, psum2 = 0, y = 0;
    for (unsigned int i = 0; i < NCAT(lengths); ++i) {
	double p1 = PROB(par1)[i];
	double p2 = PROB(par2)[i];
	if (p1 == 0) {
	    psum2 += p2;
	}
	else if (p2 == 0) {
	    return JAGS_POSINF;
	}
	else {
	    y += p1 * (log(p1) - log(p2));
	    psum1 += p1;
	    psum2 += p2;
	}
    }
    y /= psum1;
    y -= (log(psum1) - log(psum2));
    return y;
}

}
