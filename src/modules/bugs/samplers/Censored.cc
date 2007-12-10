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
using std::string;

Censored::Censored(StochasticNode *snode, Graph const &graph)
    : Sampler(vector<StochasticNode*>(1,snode), graph), _snode(snode)
{
    if (!canSample(snode, graph)) {
	throw invalid_argument("Can't construct Censored sampler");
    }

    StochasticNode const *child = stochasticChildren()[0];
    _breaks = child->parents()[1];
    _y = static_cast<int>(*child->value(0));

    if (_y < 0 || _y > _breaks->length())
	throw NodeError(_snode, "Bad interval-censored node");
}


Censored::~Censored()
{
}

bool Censored::canSample(StochasticNode *snode, Graph const &graph)
{
    // The sampler works on scalar real-valued distributions with a
    // single child: an observed stochastic node with a "dinterval"
    // distribution. 
  
    if (snode->isDiscreteValued() || snode->length() != 1)
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
    unsigned int nchain = _snode->nchain();    
    for (unsigned int n = 0; n < nchain; ++n) {

        double const *b = _breaks->value(n);
	double const *lower = (_y == 0) ? 0 : b + _y - 1;
	double const *upper = (_y == _breaks->length()) ? 0: b + _y;
	
	double x;
	_snode->distribution()->randomSample(&x, 1U, _snode->parameters(n),
                                             _snode->parameterDims(), 
                                             lower, upper, rng[n]);
	setValue(&x,1,n);
    }
}

bool Censored::isAdaptive() const
{
    return false;
}

bool Censored::adaptOff()
{
    return true;
}

string Censored::name() const
{
    return "Censored";
}
