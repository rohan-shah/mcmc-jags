#include <config.h>

#include "ConjugateFactory.h"
#include "ConjugateNormal.h"
#include "ConjugateGamma.h"
#include "ConjugateBeta.h"
#include "ConjugateDirichlet.h"
#include "ConjugateMNormal.h"
#include "ConjugateWishart.h"
#include "ConjugateSampler.h"
#include "Censored.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/Updater.h>

#include <stdexcept>
#include <string>
#include <map>

using std::string;
using std::map;
using std::invalid_argument;

ConjugateFactory::ConjugateFactory()
    : _name("Conjugate")
{
    _func_table["dnorm"] = ConjugateNormal::canSample;
    _func_table["dgamma"] = ConjugateGamma::canSample;
    _func_table["dexp"] = ConjugateGamma::canSample;
    _func_table["dchisq"] = ConjugateGamma::canSample;
    _func_table["dbeta"] = ConjugateBeta::canSample;
    _func_table["ddirch"] = ConjugateDirichlet::canSample;
    _func_table["dmnorm"] = ConjugateMNormal::canSample;
    _func_table["dwish"] = ConjugateWishart::canSample;
}

bool ConjugateFactory::canSample(StochasticNode * snode,
				 Graph const &graph) const
{
    if (Censored::canSample(snode, graph))
      return true;
    
    //FIXME: Could use a typedef here to make it readable
    map<string, bool (*)(StochasticNode *, Graph const &)>::const_iterator
	p = _func_table.find(snode->distribution()->name());

    if (p == _func_table.end())
	return false;
    else {
	return p->second(snode, graph);
    }
}

Sampler *ConjugateFactory::makeSampler(StochasticNode *snode, 
				       Graph const &graph) const
{
    Updater *updater = new Updater(snode, graph);
    ConjugateMethod* method = 0;
    
    if (Censored::canSample(snode, graph)) {
	method = new Censored(updater);
    }
    else {
	switch (getDist(snode)) {
	case NORM:
	    method = new ConjugateNormal(updater);
	    break;
	case GAMMA: case EXP: case CHISQ:
	    method = new ConjugateGamma(updater);
	    break;
	case BETA:
	    method = new ConjugateBeta(updater);
	    break;
	case DIRCH:
	    method = new ConjugateDirichlet(updater);
	    break;
	case MNORM:
	    method = new ConjugateMNormal(updater);
	    break;
	case WISH:
	    method = new ConjugateWishart(updater);
	    break;
	default:
	    throw invalid_argument("Unable to create conjugate sampler");
	}
    }
    
    
    return new ConjugateSampler(updater, method);
}

string const &ConjugateFactory::name() const
{
    return _name;
}
