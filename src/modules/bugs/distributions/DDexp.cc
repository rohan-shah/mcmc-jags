#include <config.h>
#include "DDexp.h"
#include <rng/RNG.h>

#include <cmath>

#include <JRmath.h>

using std::vector;

static inline double MU(vector<double const *> const &par)
{
    return *par[0];
}

static inline double SCALE(vector<double const *> const &par)
{
    return 1/(*par[1]);
}

DDexp::DDexp()
    : DistScalarRmath("ddexp", 2, DIST_UNBOUNDED, true, false)
{}

bool 
DDexp::checkParameterValue (vector<double const *> const &par,
			    vector<vector<unsigned int> > const &dims) const
{
  return (*par[1] > 0);
}

double 
DDexp::d(double x, vector<double const *> const &par, bool dolog) const
{
    double d = dexp(fabs(x - MU(par)), SCALE(par), dolog);
    if (dolog)
	return d - log(2.0);
    else
	return d/2;
}

double 
DDexp::p(double x, vector<double const *> const &par, bool lower, 
	 bool dolog) const
{
    if (!lower)
	x = 2 * MU(par) - x;

    double p;
    if (x < MU(par)) {
	p = pexp(MU(par) - x, SCALE(par), false, false)/2;
    }
    else {
	p = 0.5 + pexp(x - MU(par), SCALE(par), true, false)/2;
    }

    if (dolog)
	return log(p);
    else 
	return p;
}

double
DDexp::q(double p, vector<double const *> const &par, bool lower, 
	 bool log_p)
  const
{
    if (log_p)
	p = exp(p);

    if (!lower)
	p = 1 - p;

    if (p < 0.5) {
	return MU(par) - qexp(2 * (0.5 - p), SCALE(par), false, false);
    }
    else {
	return MU(par) + qexp(2 * (p - 0.5), SCALE(par), true, false);
    }
}

double DDexp::r(vector<double const *> const &par, RNG *rng) const
{
    double ans = MU(par);
    if (rng->uniform() < 0.5)
	ans -= rexp(SCALE(par), rng);
    else
	ans += rexp(SCALE(par), rng);
    return ans;
}
