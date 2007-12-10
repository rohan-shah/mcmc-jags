#include <config.h>
#include "Dirichlet.h"

#include <graph/StochasticNode.h>
#include <sampler/DensitySampler.h>
#include <rng/RNG.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

using std::vector;
using std::copy;
using std::logic_error;
using std::exp;
using std::sqrt;
using std::min;
using std::string;

DirchMetropolis::DirchMetropolis(StochasticNode* node)
    : RWMetropolis(vector<StochasticNode*>(1, node), 0.1)
{
}

void DirchMetropolis::transform(double const *v, unsigned int length,
				double *nv, unsigned int nlength) const
{
    /* We use a redundant parameterization for the Metropolis method.
     * The metropolis value v is normalized when converted to a value
     * for the sampled node.
     */
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::transform");
    }

    copy(v, v + length, nv);

    double vsum = 0;
    for (unsigned int i = 0; i < length; ++i) {
	vsum += v[i];
    }

    for (unsigned int i = 0; i < length; ++i) {
	nv[i] /= vsum;
    }
}

void DirchMetropolis::untransform(double const *nv, unsigned int nlength,
				  double *v, unsigned int length) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::untransform");
    }
    /* We work in an untransformed parameter space. I tried using a 
     * a log transformation to preserve positivity of the parameters.
     * However, this resulted in the chain getting stuck in at extreme
     * values (e.g. (0.001,0.999)) where the step size is too small
     */
    copy(nv, nv + nlength, v);
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}

void DirchMetropolis::update(RNG *rng)
{
    unsigned int d = value_length();
    double *new_value = new double[d];
    double const *old_value = value();

    //Look out for structural zeros in the alpha parameter
    StochasticNode *const snode = _sampler->nodes().front();
    double const *alpha = snode->parameters(_chain)[0];

    double step_size = step();
    double nvsum = 0;
    double vsum = 0;
    for (unsigned int i = 0; i < d; ++i) {
	if (alpha[i] > 0) {
	    new_value[i] = old_value[i] + rng->normal() * step_size;
	}
	else {
	    new_value[i] = old_value[i];
	}
	if (new_value[i] < 0) {
	    // Reflect at boundaries of the parameter space
	    new_value[i] = -new_value[i];
	}
	vsum += old_value[i];
	nvsum += new_value[i];
    }

    //Normalize values
    for (unsigned int i = 0; i < d; ++i) {
	new_value[i] /= nvsum;
    }

    /* Due to the redundant parameterization, the density does not
       depend on vsum. To stop vsum wandering off to infinity we add a
       penalty term.  This is based on a gamma likelihood with mean 1
       and shape parameter d, and keeps vsum in the region of 1.
    */

    double log_p = -_sampler->logFullConditional(_chain)
	- (d - 1) * log(vsum) + d * vsum;
    propose(new_value, d);
    log_p += _sampler->logFullConditional(_chain) 
	+ (d - 1) * log(nvsum) - d * nvsum;
    double p = exp(log_p);
    accept(rng, p);

    delete [] new_value;
}
