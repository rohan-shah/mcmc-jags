#include <config.h>
#include <distribution/Distribution.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <sampler/MutableSampler.h>
#include <sampler/GraphView.h>

#include "SumFactory.h"
#include "SumMethod.h"

#include <algorithm>

using std::list;
using std::vector;
using std::string;

namespace jags {
    namespace bugs {


	
	vector<Sampler *>
	SumFactory::makeSamplers(list<StochasticNode*> const &nodes,
				 Graph const &graph) const
	{
	    vector<Sampler*> samplers;
	    
	    vector<StochasticNode const*> sum_nodes;
	    map<StochasticNode const*, vector<StochasticNode> > smap;
	    map<StochasticNode const*, vector<StochasticNode> >::iterator i;

	    for (list<StochasticNode*>::const_iterator p = nodes.begin();
		 p != nodes.end(); ++p)
	    {
		StochasticNode const *schild = testSample(*p);
		if (schild) {
		    i = smap.find(schild);
		    if (i == smap.end()) {
			smap[schild] = vector<StochasticNode*>(1, schild);
		    }
		    else {
			sum_nodes.push_back(schild);
			i->second.push_back(*p);
		    }
		}
	    }

	    for (unsigned int i = 0; i < sum_nodes.size(); ++i) {

		i = smap.find(sum_nodes[i]);
		vector<StochasticNode*> const &parameters = i->second;
		
		bool discrete;
		bool multinom = false;

		GraphView *gv = new GraphView(parameters, graph);		
		if (!SumMethod::canSample(gv, graph)) continue;

		unsigned int nchain = parameters[0]->nchain();
		vector<MutableSampleMethod*> methods(nchain, 0);
		for (unsigned int ch = 0; ch < nchain; ++ch) {
		    methods[ch] = new SumMethod(gv, ch);
		}
		Sampler s = new MutableSampler(gv, methods, "bugs::Sum");
		samplers.push_back(s);
	    }

	    return samplers;
	}
	    

	string DSumFactory::name() const
	{
	    return "bugs::Sum";
	}

    }
}
