#include <config.h>
#include "DWeib.h"

#include <rng/RNG.h>
#include <util/nainf.h>

#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::string;

#define SHAPE(par) (*par[0])
#define SCALE(par) (pow(*par[1], -1/(*par[0])))

#define V(par) (*par[0])
#define LAMBDA(par) (*par[1])

namespace bugs {

    DWeib::DWeib()
	: RScalarDist("dweib", 2, DIST_POSITIVE)
    {}
    
    string DWeib::alias() const
    {
	return "dweibull";
    }
    
    bool 
    DWeib::checkParameterValue (vector<double const *> const &par) const
    {
	//We can get underflow and overflow when changing parameterization
	return (*par[0] > 0 && *par[1] > 0 && jags_finite(SCALE(par)) && SCALE(par) > 0);
    }
    
    double DWeib::d(double x, PDFType type,
		    vector<double const *> const &par, bool give_log) const
	
    {
	return dweibull(x, SHAPE(par), SCALE(par), give_log);
    }

    double DWeib::p(double q, vector<double const *> const &par, bool lower, 
		    bool give_log) const
    {
	return pweibull(q, SHAPE(par), SCALE(par), lower, give_log);
    }

    double DWeib::q(double p, vector<double const *> const &par, bool lower, 
		    bool log_p) const
    {
	return qweibull(p, SHAPE(par), SCALE(par), lower, log_p);
    }

    double DWeib::r(vector<double const *> const &par, RNG *rng) const
    {
	return rweibull(SHAPE(par), SCALE(par), rng);
    }

}
