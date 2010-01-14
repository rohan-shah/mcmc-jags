#include <config.h>
#include "ILogit.h"

#include <cmath>
#include <cfloat>

using std::vector;
using std::exp;
using std::log;

namespace bugs {

    ILogit::ILogit ()
	: LinkFunction ("ilogit", "logit")
    {
    }

    double ILogit::inverseLink(double eta) const
    {
	return 1/(1 + exp(-eta));
    }

    double ILogit::link(double mu) const
    {
	return log(mu) - log(1- mu);
    }

    double ILogit::grad(double eta) const
    {
	double opexp = 1 + exp(eta);
	return exp(eta) / (opexp * opexp);
    }

}
