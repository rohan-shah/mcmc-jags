#include <config.h>
#include "Dirichlet.h"

#include <graph/StochasticNode.h>
#include <sampler/ParallelDensitySampler.h>
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

DirchMetropolis::DirchMetropolis(StochasticNode* const node)
    : RWMetropolis(vector<StochasticNode*>(1, node), 1.0E-1)
{
}

void DirchMetropolis::transform(double const *v, unsigned int length,
				double *nv, unsigned int nlength) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::transform");
    }

    //Transform from log scale
    double nvsum = 0;
    for (unsigned int i = 0; i < length; ++i) {
	nv[i] = exp(v[i]);
	nvsum += nv[i];
    }

    // Normalize
    for (unsigned int i = 0; i < nlength; ++i) {
	nv[i] /= nvsum;
    }
}

void DirchMetropolis::untransform(double const *nv, unsigned int nlength,
				  double *v, unsigned int length) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::untransform");
    }

    for (unsigned int i = 0; i < length; ++i) {
	v[i] = log(nv[i]);
    }
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}

double DirchMetropolis::penalty() const
{
    unsigned int d = value_length();
    double const *v = value();
    
    double S = 0;
    for (unsigned int i = 0; i < d; ++i) {
	S += exp(v[i]);
    }
    
    StochasticNode *const snode = _sampler->nodes().front();
    double const *alpha = snode->parameters(_chain)[0];

    double alphasum = 0;
    for (unsigned int i = 0; i < d; ++i) {
        alphasum += alpha[i];
    }

    return (alphasum - 1) * log(S) - S;
}

void DirchMetropolis::update(RNG *rng)
{
    unsigned int d = value_length();
    double *new_value = new double[d];
    double const *old_value = value();

    double log_p = -_sampler->logFullConditional(_chain) - penalty();
    double s = step();

    double vsum = 0;
    for (unsigned int i = 0; i < d; ++i) {
        new_value[i] = old_value[i] + rng->normal() * s;
	vsum += new_value[i];
    }

    propose(new_value, d);
    log_p += _sampler->logFullConditional(_chain) + penalty();
    double p = exp(log_p);
    accept(rng, p);

    delete [] new_value;
}
