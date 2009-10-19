#include <config.h>

#include "Logit.h"
#include <graph/StochasticNode.h>
#include <sampler/Updater.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>
#include <cmath>

using std::vector;
using std::string;
using std::logic_error;
using std::fabs;

#define PI            3.141592653589793238462643383280 
#define PI_SQUARE     9.86960440108936

static bool r_intvl(double u, double lambda)
{
    double z = 1;
    double x = exp(-lambda/2);
    int j = 0;
    while(true) {
	j++;
	int j2 = (j+1)*(j+1);
	z -= j2 * pow(x, j2 - 1);
	if (z > u)
	    return true;
	j++;
	j2 = (j+1)*(j+1);
	z += j2 * pow(x, j2 - 1);
	if (z < u)
	    return false;
    }
}

static bool l_intvl(double u, double lambda)
{
    double h = 0.5*log(2) + 2.5*log(PI) - 2.5*log(lambda) - 
	PI_SQUARE/(2 * lambda) + 0.5*lambda;
    double logu = log(u);
    double z = 1;
    double x = exp(-PI_SQUARE/(2*lambda));
    double k = lambda / PI_SQUARE;
    int j = 0;
    while(true) {
	j++;
	z -= k*pow(x, j*j-1);
	if (h + log(z) > logu)
	    return true;
	j++;
	int j2 = (j+1)*(j+1);
	z += j2*pow(x,j2-1);
	if (h + log(z) < logu)
	    return false;
    }
}

static double sample_lambda(double delta, RNG *rng)
{
    while (true) {
	double y = rng->normal();
	y = y * y;
	y = 1 + (y - sqrt(y*(4*delta + y)))/(2*delta);
	double u = rng->uniform();
	double lambda = (u <= 1/(1+y)) ? delta/y : delta*y;
	u = rng->uniform();
	bool ok = (lambda > 2.25) ? r_intvl(u, lambda) : l_intvl(u, lambda);
	if (ok)
	    return lambda;
    }
}
	
namespace glm {

    Logit::Logit(Updater const *updater,
		 vector<Updater const *> const &sub_updaters,
		 unsigned int chain)
	: ScaleMixture(updater, sub_updaters, chain), 
	  _tau(updater->stochasticChildren().size(), 1)
    {
    }

    double Logit::getPrecision(unsigned int i) const 
    {
	return _tau[i];
    }
    
    string Logit::name() const
    {
	return "Logit";
    }
    
    void Logit::updatePrecision(RNG *rng)
    {
	for (unsigned int r = 0; r < _tau.size(); ++r)
	{
	    double delta = fabs(getValue(r) - getMean(r));
	    _tau[r] = 1/sample_lambda(delta, rng);
	}
    }

}
