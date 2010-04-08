#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/GraphView.h>
#include <rng/RNG.h>

#include "RealDSum.h"

//Initial step size
#define STEP 0.1

using std::vector;
using std::string;

static vector<double> nodeValues(GraphView const *gv, unsigned int chain)
{
    unsigned int n = gv->nodes().size();
    vector<double> ans(n);
    gv->getValue(ans, chain);

    //Correct initial value of sampled nodes to conform to the
    //constraint that they sum to dsum

    double delta = gv->stochasticChildren()[0]->value(chain)[0];
    for (unsigned int i = 0; i < n; ++i) {
	delta -= ans[i];
    }
    delta /= n;
    for (unsigned int i = 0; i < n; ++i) {
	ans[i] += delta;
    }
    gv->setValue(ans, chain);

    return(ans);
}

RealDSum::RealDSum(GraphView const *gv, unsigned int chain)
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

void RealDSum::step(vector<double> &value, double s, RNG *rng) const
{
    //Randomly draw two components of the vector
    int n = value.size();
    int i = pick(n, rng);
    int j = pick(n - 1, rng);
    if (j >= i) ++j;
	
    //Modify the chosen components while keeping the sum constant
    double eps = rng->normal() * s;
    value[i] += eps;
    value[j] -= eps;
}

string RealDSum::name() const
{
    return "RealDSum";
}


