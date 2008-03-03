#include <config.h>
#include "Exp.h"

#include <cmath>

using std::vector;
using std::exp;
using std::log;

namespace bugs {

    Exp::Exp ()
	: InverseLinkFunc ("exp", "log")
    {
    }

    double Exp::inverseLink(double eta) const
    {
	return exp(eta);
    }

    double Exp::link(double mu) const
    {
	return log(mu);
    }

    double Exp::gradLink(double mu) const
    {
	return 1/mu;
    }

}
