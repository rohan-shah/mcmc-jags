#include <config.h>

#include "Censored.h"
#include "DInterval.h"

#include <sampler/Updater.h>
#include <graph/NodeError.h>
#include <graph/StochasticNode.h>


#include <stdexcept>
#include <vector>
#include <cmath>

using std::vector;
using std::invalid_argument;
using std::logic_error;
using std::string;

int indicator(Updater const *updater, unsigned int ch)
{
    return static_cast<int>(updater->stochasticChildren()[0]->value(ch)[0]);
}

static Node const *breaks(Updater const *updater)
{
    return updater->stochasticChildren()[0]->parents()[1];
}


Censored::Censored(Updater const *updater, unsigned int chain)
    : _updater(updater), _chain(chain)
{
    int y = indicator(updater, chain);
    
    if (y < 0 || y > breaks(updater)->length())
	throw NodeError(updater->nodes()[0], "Bad interval-censored node");
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
  
    //Check that we have a single stochastic child, which is a direct
    //child of the sampled node which has distribution "dinterval"
    Updater updater(snode, graph);
    vector<StochasticNode const*> const &schild = updater.stochasticChildren();
    vector<DeterministicNode*> const &dchild = updater.deterministicChildren();

    if(schild.size() != 1)
	return false; //Too many children
    if(!dchild.empty())
	return false; //Not direct child
    if (schild[0]->distribution()->name() != "dinterval")
	return false;
    if (schild[0]->parents()[1] == snode)
	return false; //Breaks depend on snode

    return true;
}	

void Censored::update(RNG * rng)
{
    int y = indicator(_updater, _chain);
    double const *b = breaks(_updater)->value(_chain);
    int ymax = breaks(_updater)->length();

    double const *lower = (y == 0) ? 0 : b + y - 1;
    double const *upper = (y == ymax) ? 0 : b + y;
	
    double x;
    StochasticNode const *snode = _updater->nodes()[0];
    snode->distribution()->randomSample(&x, 1U, snode->parameters(_chain),
					snode->parameterDims(), 
					lower, upper, rng);
    _updater->setValue(&x, 1U, _chain);
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
