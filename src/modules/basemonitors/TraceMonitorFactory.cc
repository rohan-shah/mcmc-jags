#include "TraceMonitorFactory.h"
#include "TraceMonitor.h"

#include <model/Model.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace basemonitors {

    Monitor *TraceMonitorFactory::getMonitor(Node const *node, 
						Model *model,
						unsigned int start,
						unsigned int thin, 
						string const &type)
    {
	if (type != "trace")
	    return 0;

	return new TraceMonitor(node, start, thin);
    }

    vector<Node const*> 
    TraceMonitorFactory::defaultNodes(Model *model, 
					 string const &type) const
    {
	vector<Node const*> dnodes;

        if (type == "trace") {
	    set<Node*> const &nodes = model->graph().nodes();
	    set<Node*>::const_iterator p = nodes.begin(); 
	    for ( ; p != nodes.end(); ++p) {
		StochasticNode const *snode = asStochastic(*p);
		if (snode) {
		    //Find stochastic nodes with observed parents
		    bool istop = true;
		    vector<Node const*> const &parents = (*p)->parents();
		    vector<Node const*>::const_iterator q;
		    for (q = parents.begin(); q != parents.end(); ++q) {
			if (!(*q)->isObserved()) {
			    istop = false;
			    break;
			}
		    }
		    if (istop) {
			dnodes.push_back(*p);
		    }
		}
	    }
	}
	return dnodes;
    }
	
}
