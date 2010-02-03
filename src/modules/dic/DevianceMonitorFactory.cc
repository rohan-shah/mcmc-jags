#include "DevianceMonitorFactory.h"
#include "DevianceMonitor.h"

#include <model/Model.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace dic {

    Monitor *DevianceMonitorFactory::getMonitor(Node const *node, 
						Model *model,
						string const &type)
    {
	if (type != "deviance")
	    return 0;
	if (!node->isObserved()) 
	    return 0;

	StochasticNode const *snode = asStochastic(node);
	if (snode)
	    return new DevianceMonitor(snode);
	else 
	    return 0;
    }

    vector<Node const*> 
    DevianceMonitorFactory::defaultNodes(Model *model, 
					 string const &type) const
    {
	vector<Node const*> dnodes;

        if (type == "deviance") {
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
