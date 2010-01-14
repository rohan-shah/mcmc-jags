#include <config.h>
#include "Exp.h"

#include <cmath>

using std::vector;
using std::exp;
using std::log;

namespace bugs {

    Exp::Exp ()
	: LinkFunction ("exp", "log")
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

    double Exp::grad(double eta) const
    {
	return exp(eta);
    }

}
