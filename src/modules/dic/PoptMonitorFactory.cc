#include "PoptMonitorFactory.h"
#include "PoptMonitor.h"

#include <model/BUGSModel.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace dic {

    Monitor *PoptMonitorFactory::getMonitor(string const &name, 
					    Range const &range,
					    BUGSModel *model,
					    string const &type)
    {
	if (type != "popt" || model->nchain() < 2)
	    return 0;

	Node *node = model->getNode(name, range);
	if (!node)
	    return 0;

	unsigned int nchain = model->nchain();
	vector<RNG*> rngs;
	for (unsigned int i = 0; i < nchain; ++i) {
	    rngs.push_back(model->rng(i));
	}

	StochasticNode const *snode = asStochastic(node);
	if (snode)
	    return new PoptMonitor(snode, rngs, 10);
	else 
	    return 0;
    }

    vector<Node const*> 
    PoptMonitorFactory::defaultNodes(Model *model, string const &type) const
    {
	vector<Node const*> dnodes;

        if (type == "popt" && model->nchain() >= 2) {
	    set<Node*> const &nodes = model->graph().nodes();
	    set<Node*>::const_iterator p = nodes.begin(); 
	    for ( ; p != nodes.end(); ++p) {
		if ((*p)->isObserved() && asStochastic(*p)) {
		    dnodes.push_back(*p);
		}
	    }
	}
	return dnodes;
    }
	
}
