#include <config.h>

#include "ConjugateFactory.h"
#include "ConjugateNormal.h"
#include "ConjugateGamma.h"
#include "ConjugateBeta.h"
#include "ConjugateDirichlet.h"
#include "ConjugateMNormal.h"
#include "ConjugateWishart.h"
#include "ConjugateSampler.h"
//#include "Censored.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include <stdexcept>
#include <string>
#include <map>

using std::string;
using std::map;
using std::invalid_argument;

ConjugateFactory::ConjugateFactory()
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
    /*
      if (Censored::canSample(snode, graph))
      return true;
    */

    //FIXME: Could use a typedef here to make it readable
    map<const string, bool (*)(StochasticNode *, Graph const &)>::const_iterator
	p = _func_table.find(snode->distribution()->name());

    if (p == _func_table.end())
	return false;
    else {
	return p->second(snode, graph);
    }
}

Sampler *ConjugateFactory::makeSingletonSampler(StochasticNode *snode, 
						Graph const &graph) const
{
    /*
      if (Censored::canSample(snode, graph))
      return new Censored(snode, graph);
    */
    
    unsigned int nchain = snode->nchain();

    ConjugateMethod* method = 0;
    switch (getDist(snode)) {
    case NORM:
	method = new ConjugateNormal();
	break;
    case GAMMA: case EXP: case CHISQ:
	method = new ConjugateGamma();
	break;
    case BETA:
	method = new ConjugateBeta();
	break;
    case DIRCH:
	method = new ConjugateDirichlet();
	break;
    case MNORM:
	method = new ConjugateMNormal();
	break;
    case WISH:
	method = new ConjugateWishart();
	break;
    default:
	throw invalid_argument("Unable to create conjugate sampler");
    }
    
    return new ConjugateSampler(snode, graph, method);
}
