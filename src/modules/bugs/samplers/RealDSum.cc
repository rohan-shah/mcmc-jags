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

//Pick a random integer between 0 and n - 1
static int pick(int n, RNG *rng)
{
    double u = rng->uniform() * n;
    int i = 1;
    while (i < u) ++i;
    return i - 1;
}

namespace bugs {

RealDSum::RealDSum(GraphView const *gv, unsigned int chain)
    : RWDSum(gv, chain, STEP)
{
}

void RealDSum::step(vector<double> &value, unsigned int nrow,
		    unsigned int ncol, double s, RNG *rng) const
{
    //Randomly pick a row
    int r = pick(nrow, rng);

    //Randomly draw two columns 
    int c1 = pick(ncol, rng);
    int c2 = pick(ncol - 1, rng);
    if (c2 >= c1) ++c2;
    
    //Modify the chosen components while keeping the sum constant
    double eps = rng->normal() * s;
    value[c1 * nrow + r] += eps;
    value[c2 * nrow + r] -= eps;
}

string RealDSum::name() const
{
    return "RealDSum";
}

}


