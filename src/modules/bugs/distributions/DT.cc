#include <config.h>
#include "DT.h"

#include <cmath>

#include <JRmath.h>

using std::vector;

#define MU(par) (*par[0])
#define TAU(par) (*par[1])
#define DF(par) (*par[2])

namespace bugs {

DT::DT()
    : RScalarDist("dt", 3, DIST_UNBOUNDED)
{}

bool DT::checkParameterValue (vector<double const *> const &par) const
{
    return (TAU(par) > 0 && DF(par) > 0);
}

double DT::d(double x, PDFType type,
vector<double const *> const &par, bool give_log) const
{
    x = (x - MU(par)) * sqrt(TAU(par));
    if (type == PDF_PRIOR) {
	double y = - (DF(par) + 1) * log(1 + x * x / DF(par)) / 2;
	return give_log ? y : exp(y);
    }
    else if (give_log) {
	return dt(x, DF(par), 1) + log(TAU(par))/2;
    }
    else {
	return dt(x, DF(par), 0) * sqrt(TAU(par));
    }
}

double DT::p(double x, vector<double const *> const &par, bool lower, 
	     bool use_log) const
{
    return pt((x - MU(par)) * sqrt(TAU(par)), DF(par), lower, use_log);
}

double DT::q(double p, vector<double const *> const &par, bool lower, 
	     bool log_p) const
{
    return MU(par) + qt(p, DF(par), lower, log_p) / sqrt(TAU(par));
}

double DT::r(vector<double const *> const &par, RNG *rng) const
{
    return rt(DF(par), rng) / sqrt(TAU(par)) + MU(par);
}

}
