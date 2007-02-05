#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <rng/RNG.h>
#include <sarray/nainf.h>
#include <graph/NodeError.h>
#include "FiniteSampler.h"

#include <cmath>
#include <string>
#include <stdexcept>
#include <vector>

using std::logic_error;
using std::string;
using std::vector;

FiniteSampler::FiniteSampler(StochasticNode *node, Graph const &graph, 
			     unsigned int chain)
  : Sampler(vector<StochasticNode*>(1,node), graph), _chain(chain)
{
    if (!canSample(node, graph))
	throw logic_error("Attempt to construct invalid FiniteSampler");
    
    Distribution const *dist = node->distribution();
    
    double lower = 0, upper = 0;
    dist->support(&lower, &upper, 1, node->parameters(chain), node->parameterDims());
    _lower = static_cast<int>(lower);
    _upper = static_cast<int>(upper);
}

FiniteSampler::~FiniteSampler()
{}

void FiniteSampler::update(RNG *rng)
{
    int size = _upper - _lower + 1;
    double *lik = new double[size];
    double liksum = 0.0;
    for (int i = 0; i < size; i++) {
	double ivalue = _lower + i;
	setValue(&ivalue, 1, _chain);
	lik[i] = exp(logFullConditional(_chain));
	liksum += lik[i];
    }

    if (liksum == 0) {
	throw NodeError(nodes()[0], "All possible values have zero probability in FiniteSampler");
    }
    if (!jags_finite(liksum)) {
	throw NodeError(nodes()[0], "Cannot normalize density in FiniteSampler");
    }

    /* Sample */
    double urand = rng->uniform() * liksum;
    int i;
    liksum = 0.0;
    for (i = 0; i < size - 1; i++) {
	liksum += lik[i];
	if (liksum > urand) {
	    break;
	}
    }
    double ivalue = _lower + i;
    setValue(&ivalue, 1, _chain);
    delete [] lik;
}

bool FiniteSampler::canSample(StochasticNode const *node,
			      Graph const &graph)
{
    //Node must be scalar with discrete-valued distribution of full rank
    Distribution const *dist = node->distribution();
    if (!dist->isDiscreteValued())
	return false;

    if (node->length() != 1)
	return false;

    if (df(node) == 0)
	return false;

    for (unsigned int ch = 0; ch < node->nchain(); ++ch) {
	//Distribution cannot be unbounded
	double ulimit = 0, llimit = 0;
	dist->support(&llimit, &ulimit, 1, node->parameters(ch), 
		  node->parameterDims());
	if (!jags_finite(ulimit) || !jags_finite(llimit))
	    return false;

	//We don't want too many possibilities
	double n = ulimit - llimit + 1;
	if (n <= 1 || n > 20) //fixme: totally arbitrary
	    return false;

	//Support must be fixed
	vector<bool> fixmask(node->parents().size());
	for (unsigned int i = 0; i < node->parents().size(); ++i) {
	    fixmask[i] = node->parents()[i]->isObserved();
	}
	if (!node->distribution()->isSupportFixed(fixmask))
	    return false;
    }
    return true;
}

bool FiniteSampler::adaptOff()
{
    return false;
}
