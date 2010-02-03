#include "TraceMonitorFactory.h"
#include "TraceMonitor.h"

#include <model/Model.h>
#include <graph/Graph.h>
#include <graph/Node.h>

using std::set;
using std::string;
using std::vector;

namespace base {

    Monitor *TraceMonitorFactory::getMonitor(Node const *node, 
					     Model *model,
					     string const &type)
    {
	if (type != "trace")
	    return 0;

	return new TraceMonitor(node);
    }

    vector<Node const*> 
    TraceMonitorFactory::defaultNodes(Model *model, 
				      string const &type) const
    {
	vector<Node const*> dnodes;

        if (type == "trace") {
	    set<Node*> const &nodes = model->graph().nodes();
	    set<Node*>::const_iterator p;
	    for (p = nodes.begin() ; p != nodes.end(); ++p) {
		//Find random variables with observed parents
		if ((*p)->isRandomVariable() && !(*p)->isObserved()) {
		    vector<Node const*> const &parents = (*p)->parents();
		    bool istop = true;
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
