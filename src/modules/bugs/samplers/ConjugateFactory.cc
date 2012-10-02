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
//#include "TruncatedGamma.h"
#include "ShiftedCount.h"
#include "ShiftedMultinomial.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/GraphView.h>
#include <module/ModuleError.h>

#include <string>

using std::string;

namespace bugs {

bool ConjugateFactory::canSample(StochasticNode * snode,
				 Graph const &graph) const
{
/*
    if (Censored::canSample(snode, graph))
      return true;
*/
    bool ans = false;
    switch(getDist(snode)) {
    case NORM:
	ans = ConjugateNormal::canSample(snode, graph);
	break;
    case GAMMA: case CHISQ:
	ans = ConjugateGamma::canSample(snode, graph);
	break;
    case EXP:
	ans = ConjugateGamma::canSample(snode, graph) ||
	    ConjugateNormal::canSample(snode, graph);
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
    case UNIF:
        /*
	ans = TruncatedGamma::canSample(snode, graph) ||
	      ConjugateBeta::canSample(snode, graph);
        */
	ans = ConjugateBeta::canSample(snode, graph);
	break;
    case POIS: case BIN: case NEGBIN:
	ans = ShiftedCount::canSample(snode, graph);
	break;
    case MULTI:
	ans = ShiftedMultinomial::canSample(snode, graph);
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
    
/*
    if (Censored::canSample(snode, graph)) {
	method = new Censored(gv);
    }
    else {
*/
	switch (getDist(snode)) {
	case NORM:
	    method = new ConjugateNormal(gv);
	    break;
	case GAMMA: case CHISQ:
	    method = new ConjugateGamma(gv);
	    break;
	case EXP:
	    if (ConjugateGamma::canSample(snode, graph)) {
		method = new ConjugateGamma(gv);
	    }
	    else if (ConjugateNormal::canSample(snode, graph)) {
		method = new ConjugateNormal(gv);
	    }
	    else {
		throwLogicError("Cannot find conjugate sampler for exponential");
	    }
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
	case UNIF:
	  /*
	    if (TruncatedGamma::canSample(snode, graph)) {
		method = new TruncatedGamma(gv);
	    }
	    else 
	  */
	    if (ConjugateBeta::canSample(snode, graph)) {
		method = new ConjugateBeta(gv);
	    }
	    else {
		throwLogicError("Cannot find conjugate sampler for uniform");
	    }
	    break;
	case POIS: case BIN: case NEGBIN:
	    method = new ShiftedCount(gv);
	    break;
	case MULTI:
	    method = new ShiftedMultinomial(gv);
	    break;
	default:
	    throwLogicError("Unable to create conjugate sampler");
	}
/* 
   }
*/  
    
    return new ConjugateSampler(gv, method);
}

string ConjugateFactory::name() const
{
    return "bugs::Conjugate";
}

}
