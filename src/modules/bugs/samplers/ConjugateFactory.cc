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
#include <sampler/GraphView.h>

#include <stdexcept>
#include <string>
#include <map>

using std::string;
using std::map;
using std::invalid_argument;

ConjugateFactory::ConjugateFactory()
    : _name("Conjugate")
{
}

bool ConjugateFactory::canSample(StochasticNode * snode,
				 Graph const &graph) const
{
    if (Censored::canSample(snode, graph))
      return true;

    bool ans = false;
    switch(getDist(snode)) {
    case NORM:
	ans = ConjugateNormal::canSample(snode, graph);
	break;
    case GAMMA: case EXP: case CHISQ:
	ans = ConjugateGamma::canSample(snode, graph);
	break;
    case BETA:
	ans = ConjugateBeta::canSample(snode, graph);
	break;
    case DIRCH:
	ans = ConjugateDirichlet::canSample(snode, graph);
	break;
    case MNORM:
	ans = ConjugateMNormal::canSample(snode, graph);
	break;
    case WISH:
	ans = ConjugateWishart::canSample(snode, graph);
	break;
    default:
	break;
    }
    
    return ans;
}

Sampler *ConjugateFactory::makeSampler(StochasticNode *snode, 
				       Graph const &graph) const
{
    GraphView *gv = new GraphView(snode, graph);
    ConjugateMethod* method = 0;
    
    if (Censored::canSample(snode, graph)) {
	method = new Censored(gv);
    }
    else {
	switch (getDist(snode)) {
	case NORM:
	    method = new ConjugateNormal(gv);
	    break;
	case GAMMA: case EXP: case CHISQ:
	    method = new ConjugateGamma(gv);
	    break;
	case BETA:
	    method = new ConjugateBeta(gv);
	    break;
	case DIRCH:
	    method = new ConjugateDirichlet(gv);
	    break;
	case MNORM:
	    method = new ConjugateMNormal(gv);
	    break;
	case WISH:
	    method = new ConjugateWishart(gv);
	    break;
	default:
	    throw invalid_argument("Unable to create conjugate sampler");
	}
    }
    
    
    return new ConjugateSampler(gv, method);
}

string const &ConjugateFactory::name() const
{
    return _name;
}
