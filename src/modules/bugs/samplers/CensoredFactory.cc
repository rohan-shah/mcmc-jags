#include <config.h>

#include "CensoredFactory.h"
#include "Censored.h"

#include <sampler/SingletonGraphView.h>
#include <sampler/ImmutableSampler.h>

using std::string;

namespace jags {
    namespace bugs {

	bool CensoredFactory::canSample(StochasticNode * snode,
					 Graph const &graph) const
	{
	    return Censored::canSample(snode, graph);
	}

	Sampler *CensoredFactory::makeSampler(StochasticNode *snode, 
					       Graph const &graph) const
	{
	    
	    SingletonGraphView *gv = new SingletonGraphView(snode, graph);
	    ImmutableSampleMethod* method = new Censored(gv);
	    return new ImmutableSampler(gv, method, "bugs::Censored");
	}
	
	string CensoredFactory::name() const
	{
	    return "bugs::Censored";
	}
	
    }
}
