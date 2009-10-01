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


Censored::Censored(Updater const *updater)
    : ConjugateMethod(updater)
{
    int nbreaks = breaks(updater)->length();
    StochasticNode const *snode = updater->nodes()[0];
    for (unsigned int ch = 0; ch < snode->nchain(); ++ch) {
	int y = indicator(updater, ch);
	if (y < 0 || y > nbreaks) {
	    throw NodeError(snode, "Bad interval-censored node");
	}
    }
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

void Censored::update(Updater *updater, unsigned int chain, RNG * rng) const
{
    int y = indicator(updater, chain);
    double const *b = breaks(updater)->value(chain);
    int ymax = breaks(updater)->length();

    double const *lower = (y == 0) ? 0 : b + y - 1;
    double const *upper = (y == ymax) ? 0 : b + y;
	
    double x;
    StochasticNode const *snode = updater->nodes()[0];
    snode->distribution()->randomSample(&x, 1U, snode->parameters(chain),
					snode->parameterDims(), 
					lower, upper, rng);
    updater->setValue(&x, 1U, chain);
}

string Censored::name() const
{
    return "Censored";
}
