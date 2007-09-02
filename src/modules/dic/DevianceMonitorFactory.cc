#include "DevianceMonitorFactory.h"
#include "DevianceMonitor.h"

#include <model/Model.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;

namespace dic {

    Monitor *DevianceMonitorFactory::getMonitor(Node const *node, 
						unsigned int start,
						unsigned int thin, 
						std::string const &type)
    {
	if (type != "deviance")
	    return 0;
	if (!node->isObserved()) 
	    return 0;

	StochasticNode const *snode = asStochastic(node);
	if (!snode)
	    return 0;

	return new DevianceMonitor(snode, start, thin);
    }

    //FIXME: Who owns these monitors?
    void DevianceMonitorFactory::addDefaultMonitors(Model *model,
                                                    unsigned int thin,
			                            std::string const &type)
    {
        if (type != "deviance") {
            return;
        }

	unsigned int start = model->iteration() + 1;

	set<Node*> const &nodes = model->graph().nodes();
	set<Node*>::const_iterator p = nodes.begin(); 
	for ( ; p != nodes.end(); ++p) {
	    if ((*p)->isObserved()) {
		StochasticNode const *snode = asStochastic(*p);
		if (snode) {
		    Monitor *monitor = new DevianceMonitor(snode, start, thin);
		    model->addMonitor(monitor);
		}
	    }
	}
    }
	
}
