#include <config.h>
#include "Dirichlet.h"

#include <graph/StochasticNode.h>
#include <sampler/Updater.h>
#include <rng/RNG.h>

#include <cmath>
#include <algorithm>
#include <cfloat>

using std::copy;
using std::exp;
using std::log;
using std::string;
using std::vector;

static vector<double> initValue(Updater const *updater, unsigned int chain)
{
    vector<double> ivalue(updater->length());
    updater->getValue(ivalue, chain);
    return ivalue;
}

DirchMetropolis::DirchMetropolis(Updater const *updater, unsigned int chain)
    : RWMetropolis(initValue(updater, chain), 0.1),
      _updater(updater), _chain(chain)
{
}

/* 
   Generates a reversible transition from xold to xnew in detailed
   balance with the exponential distribution (mean = 1).

   The proposal is generated from a nested Metropolis-Hastings
   algorithm.  
*/
static double nextPoint(double xold, double s, RNG *rng, int max_step = 10)
{
    // Do a random walk on the log scale. The acceptance probability
    // includes a Jacobian term to account for the log transform.
    for (int i = 0; i < max_step; ++i) {
        double xnew = xold * exp(s * rng->normal());
	double R = exp(xold - xnew) * (xnew / xold);
	if (rng->uniform() < R)
	    return xnew;
    }
    return xold;
}

/* 
   We want to generate a random walk on the N-dimensional simplex X[1]
   ... X[N].  The constraint sum(X) = 1 makes this hard, so we embed
   the problem in a higher-dimensional model.

   Y[i] ~ dexp(1) for i = 1 ... N
   S = sum(Y)
   X[i] = Y[i]/S

   The nextPoint function generates a reversible transition for a single
   element Y[i].

   The algorithm rests on the fact that X ~ Dirichlet(1,1,...1) (so
   that p(X) = 1) and S is independent of X. So after generating
   a proposal in the larger model (Y) we can renormalize to get a
   proposal on the simplex X such that P(X -> Xnew) = P(Xnew -> X).
*/
void DirchMetropolis::step(vector<double> &value,  double s, RNG *rng) const
{
    double S = 0;
    for (unsigned int i = 0; i < value.size(); ++i) {
	//Update as if value[i] was a sample from dexp(1)
	if (value[i] != 0) {
	    value[i] = nextPoint(value[i], s, rng);
	    S += value[i];
	}
    }
    for (unsigned int i = 0; i < value.size(); ++i) {
	//Renormalize
	if (value[i] != 0) {
	    value[i] /= S;
	}
    }
}

void DirchMetropolis::getValue(vector<double> &value) const
{
    double const *v = _updater->nodes()[0]->value(_chain);
    copy(v, v + _updater->length(), value.begin());
}

void DirchMetropolis::setValue(vector<double> const &value)
{
    _updater->setValue(value, _chain);
}

double DirchMetropolis::logDensity() const
{
    return _updater->logFullConditional(_chain);
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}
