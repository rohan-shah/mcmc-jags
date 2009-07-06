#include <config.h>
#include "DCut.h"
//#include <util/dim.h>
//#include <util/nainf.h>

#include <algorithm>

using std::vector;
using std::copy;

DCut::DCut()
    : Distribution("dinterval", 1, false, true)
{
}

double 
DCut::logLikelihood(double const *y, unsigned int length, 
			 vector<double const *> const &par,
			 vector<vector<unsigned int> > const &dims,
			 double const *lower, double const *upper) const
{
    return 0;
}

void DCut::randomSample(double  *x, unsigned int length,
			     vector<double const *> const &par,
			     vector<vector<unsigned int> > const &dims,
			     double const *lower, double const *upper,
			     RNG *rng) const
{
    // DCut simply copies its parent's value
    copy(par[0], par[0] + length, x);
}

void DCut::typicalValue(double *x, unsigned int length,
			     vector<double const *> const &par,
			     vector<vector<unsigned int> > const &dims,
			     double const *lower, double const *upper) const
{
    // DCut simply copies its parent's value
    copy(par[0], par[0] + length, x);
}

unsigned int DCut::df(vector<vector<unsigned int> > const &dims) const
{
    return 0;
}

void DCut::support(double *lower, double *upper, unsigned int length,
			std::vector<double const *> const &par,
			vector<vector<unsigned int> > const &dims) const
{
    copy(par[0], par[0] + length, lower);
    copy(par[0], par[0] + length, upper);
}


bool DCut::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0];
}
