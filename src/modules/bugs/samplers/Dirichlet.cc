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
    : RWMetropolis(vector<StochasticNode*>(1, node), 1)
{
}

void DirchMetropolis::transform(double const *v, unsigned int length,
				double *nv, unsigned int nlength) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::transformValues");
    }
    double vmean = 0;
    for (unsigned int i = 0; i < length; ++i) {
	vmean += v[i]/length;
    }
    double nvsum = 0;
    for (unsigned int i = 0; i < length; ++i) {
	nv[i] = exp(v[i] - vmean);
	nvsum += nv[i];
    }
    for (unsigned int i = 0; i < length; ++i) {
	nv[i] /= nvsum;
    }
}

void DirchMetropolis::untransform(double const *nv, unsigned int nlength,
				  double *v, unsigned int length) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in DirchMetropolis::transformValues");
    }
    double vsum = 0;
    for (unsigned int i = 0; i < nlength; ++i) {
	v[i] = log(nv[i]);
	vsum += v[i];
    }
    for (unsigned int i = 0; i < nlength; ++i) {
	v[i] -= vsum/nlength;
    }
}

string DirchMetropolis::name() const
{
    return "DirchMetropolis";
}
