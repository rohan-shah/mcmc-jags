#include "TraceMonitorFactory.h"
#include "TraceMonitor.h"

#include <model/Model.h>
#include <graph/StochasticNode.h>
#include <graph/NodeSet.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace base {

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
	    StochasticNodeSet const &snodes = 
		model->graph().stochasticNodes();
	    StochasticNodeSet::const_iterator p ;
	    for (p = snodes.begin() ; p != snodes.end(); ++p) {
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
	return dnodes;
    }
	
}
