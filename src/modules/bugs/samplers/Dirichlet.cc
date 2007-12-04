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
    : RWMetropolis(vector<StochasticNode*>(1, node), 0.01)
{
}

void DirchMetropolis::transform(double const *v, unsigned int length,
				double *nv, unsigned int nlength) const
{
    if (length != nlength - 1) {
	throw logic_error("Invalid length in DirchMetropolis::transform");
    }

    double vextra = 0;
    for (unsigned int i = 0; i < length; ++i) {
	vextra -= v[i];
    }
    vextra /= length;

    double nvsum = 0;
    for (unsigned int i = 0; i < length; ++i) {
	nv[i] = exp(v[i]);
	nvsum += nv[i];
    }
    nv[length] = exp(vextra);
    double nvsum += nv[length];

    
    for (unsigned int i = 0; i < length; ++i) {
	nv[i] /= nvsum;
    }
    nv[length] /= nvsum;
}

void DirchMetropolis::untransform(double const *nv, unsigned int nlength,
				  double *v, unsigned int length) const
{
    if (length != nlength - 1) {
	throw logic_error("Invalid length in DirchMetropolis::untransform");
    }

    double vsum = 0;
    for (unsigned int i = 0; i < length; ++i) {
	v[i] = log(nv[i]);
	vsum += v[i];
    }
    vsum += log(nv[length]);

    double vmean = vsum/nlength;
    for (unsigned int i = 0; i < length; ++i) {
	v[i] -= vmean;
    }
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}
