#include "TraceMonitorFactory.h"
#include "TraceMonitor.h"

#include <model/BUGSModel.h>
#include <graph/Graph.h>
#include <graph/Node.h>
#include <sarray/RangeIterator.h>

using std::set;
using std::string;
using std::vector;

namespace base {

    Monitor *TraceMonitorFactory::getMonitor(string const &name,
					     Range const &range,
					     BUGSModel *model,
					     string const &type)
    {
	if (type != "trace")
	    return 0;

	Node *node = model->getNode(name, range);
	if (!node) {
	    return 0;
	}
	
	TraceMonitor *m = new TraceMonitor(node);
	
	//Set name attributes 
	m->setName(name + print(range));
	Range node_range = range;
	if (isNULL(range)) {
	    //Special syntactic rule: a null range corresponds to the whole
	    //array
	    NodeArray const *array = model->symtab().getVariable(name);
	    node_range = array->range();
	}
	vector<string> elt_names;
	if (node_range.length() > 1) {
	    for (RangeIterator i(node_range); !i.atEnd(); i.nextLeft()) {
		elt_names.push_back(name + print(i));
	    }
	}
	else {
	    elt_names.push_back(name + print(range));
	}
	m->setElementNames(elt_names);
	
	return m;
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
