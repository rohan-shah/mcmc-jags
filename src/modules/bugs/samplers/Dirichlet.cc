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

void DirchMetropolis::step(vector<double> &value,  double s, RNG *rng) const
{
    double sumv = 0;
    for (unsigned int i = 0; i < value.size(); ++i) {
	if (value[i] != 0) {
	    value[i] = exp(log(value[i]) + rng->normal() * s);
	    if (value[i] == 0) {
		value[i] = 16 * DBL_EPSILON; //underflow
	    }
	    sumv += value[i];
	}
    }
    for (unsigned int i = 0; i < value.size(); ++i) {
	value[i] /= sumv;
    }
}

double DirchMetropolis::logJacobian(vector<double> const &value) const
{
    double ljacobian = 0;
    for (unsigned int i = 0; i < value.size(); ++i) {
	if (value[i] != 0) {
	    ljacobian += log(value[i]);
	}
    }
    return ljacobian;
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
