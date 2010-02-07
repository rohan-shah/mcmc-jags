#include "DevianceMonitorFactory.h"
#include "DevianceMonitor.h"

#include <model/BUGSModel.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace dic {

    Monitor *DevianceMonitorFactory::getMonitor(string const &name, 
						Range const &range,
						BUGSModel *model,
						string const &type)
    {
	if (type != "deviance")
	    return 0;

	Node const *node = model->getNode(name, range);
	if (!node || !node->isObserved()) 
	    return 0;

	StochasticNode const *snode = asStochastic(node);
	if (!snode)
	    return 0;

	Monitor *m = new DevianceMonitor(snode);
	m->setName(name + print(range));
	m->setElementNames(vector<string>(1,name+print(range)));
	return m;
    }
	
}
