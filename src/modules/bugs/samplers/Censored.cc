#include <config.h>

#include "Censored.h"
#include "DInterval.h"
//#include "RNG.h"

#include <graph/NodeError.h>
#include <graph/StochasticNode.h>

#include <stdexcept>
#include <vector>
#include <cmath>

using std::vector;
using std::invalid_argument;
using std::logic_error;

Censored::Censored(StochasticNode *snode, Graph const &graph)
    : Sampler(vector<StochasticNode*>(1,snode), graph), _snode(snode),
      _parameters(snode->nchain())
{
    if (!canSample(snode, graph)) {
	throw invalid_argument("Can't construct Censored sampler");
    }

    unsigned int nchain = snode->nchain();
    for (unsigned int n = 0; n < nchain; ++n) {
	_parameters[n] = snode->parameters(n);
    }
}

Censored::~Censored()
{
}

bool Censored::canSample(StochasticNode *snode, Graph const &graph)
{
    // The sampler works on scalar real-valued distributions with a
    // single child: an observed stochastic node with a "dinterval"
    // distribution. 
  
    if (snode->distribution()->isDiscreteValued() || snode->length() != 1)
	return false;

    // The sampler relies on the fact that boundable distributions can
    // draw truncated random samples. However, the node itself must not
    // be bounded as we don't want the additional complication of combining
    // a priori and a posteriori bounds on the distribution.
    if (!snode->distribution()->canBound())
	return false;
    if (isBounded(snode))
	return false;
  
    if (snode->children()->size() != 1)
	return false;

    Node const *child = *snode->children()->begin();
    if (!child->isObserved())
	return false;

    StochasticNode const *schild = asStochastic(child);
    return (schild && schild->distribution()->name() == "dinterval");
}	

void Censored::update(vector<RNG *> const &rng)
{
    StochasticNode const *child = stochasticChildren()[0];
    unsigned int nbreak = child->parents()[1]->length();

    unsigned int nchain = _snode->nchain();    
    for (unsigned int n = 0; n < nchain; ++n) {

	double const *breaks = child->parameters(n)[1];

	int y = static_cast<int>(child->value(n)[0]);
	if (y < 0 || y > nbreak)
	    throw NodeError(_snode, "Bad interval-censored node");
	
	double const *lower = (y == 0) ? 0 : breaks + y - 1;
	double const *upper = (y == nbreak) ? 0: breaks + y;
    
	double x;
	_snode->distribution()->randomSample(&x, 1, _parameters[n],
                                             _snode->parameterDims(), 
                                             lower, upper, rng[n]);
	setValue(&x,1,n);
    }
}

bool Censored::adaptOff()
{
    return true;
}
