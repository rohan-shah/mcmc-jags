#include <config.h>
#include "Dirichlet.h"

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
      _updater(updater), _chain(chain), _S(1)
{
}

void DirchMetropolis::step(vector<double> &value,  double step, RNG *rng) const
{
    //Random walk on a log scale. 
    for (unsigned int i = 0; i < value.size(); ++i) {
	value[i] *= exp(step * rng->normal());
    }
}

void DirchMetropolis::getValue(vector<double> &value) const
{
    _updater->getValue(value, _chain);
    for (unsigned int i = 0; i < value.size(); ++i) {
	value[i] *= _S;
    }
}

void DirchMetropolis::setValue(vector<double> const &value)
{
    double S = 0;
    for (unsigned int i = 0; i < value.size(); ++i) {
	S += value[i];
    }
    //Make a scaled down copy of the value
    vector<double> v = value;
    for (unsigned int i = 0; i < v.size(); ++i) {
	v[i] /= S;
    }

    _updater->setValue(v, _chain);
    _S = S;
}

double DirchMetropolis::logDensity() const
{
    // We need to add a penalty based on _S to stop the random walk
    // going off to infinity. This penalty is based on a normal 
    // distribution for log(_S) with mean 0 and variance 0.1.
    
    double logS = log(_S);
    return _updater->logFullConditional(_chain) - 5 * logS * logS;
}

double DirchMetropolis::logJacobian(vector<double> const &value) const
{
    //Compensate for random walk on log scale

    double lj = 0;
    for (unsigned int i = 0; i < value.size(); ++i) {
        if (value[i]) {
	   lj += log(value[i]);
        }
    }
    return lj;
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}
