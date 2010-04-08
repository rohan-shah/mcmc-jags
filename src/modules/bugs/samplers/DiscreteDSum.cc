#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/GraphView.h>
#include <rng/RNG.h>

#include "DiscreteDSum.h"

#include <cmath>
#include <stdexcept>

using std::vector;
using std::fabs;
using std::logic_error;
using std::string;

//Initial step size
#define STEP 1

static vector<double> nodeValues(GraphView const *gv, unsigned int chain)
{
    unsigned int n = gv->nodes().size();
    vector<double> ans(n);
    gv->getValue(ans, chain);

   //Ensure that values of sampled nodes are consistent with dsum parent

    double delta = gv->stochasticChildren()[0]->value(chain)[0];
    for (unsigned int i = 0; i < n; ++i) {
	delta -= ans[i];
    }

    if (delta != 0) {
	int idelta = static_cast<int>(delta);
	if (delta != idelta) {
	    throw logic_error("Unable to satisfy dsum constraint");
	}
	int eps = idelta / n;
	int resid = idelta % n;
	
	for (unsigned int i = 0; i < n; ++i) {
	    ans[i] += eps;
	}
	ans[0] += resid;
	gv->setValue(ans, chain);
    }

    return ans;
}

DiscreteDSum::DiscreteDSum(GraphView const *gv, unsigned int chain)
    : RWDSum(nodeValues(gv, chain), STEP, gv, chain)
{
}

//Pick a random integer between 0 and n - 1
static int pick(int n, RNG *rng)
{
    double u = rng->uniform() * n;
    int i = 1;
    while (i < u) ++i;
    return i - 1;
}
void DiscreteDSum::step(vector<double> &value, double s, RNG *rng) const
{
    int n = value.size();

    //Randomly pick two elements of the value vector
    int i = pick(n, rng);
    int j = pick(n - 1, rng);
    if (j >= i) ++j;

    //Modify the chosen elements while keeping the sum constant
    double eps = rng->normal() * s;
    int inteps = static_cast<int>(fabs(eps)) + 1;
    if (eps < 0) {
	inteps = -inteps;
    }
    value[i] += inteps;
    value[j] -= inteps;
}

string DiscreteDSum::name() const
{
    return "DiscreteDSum";
}

