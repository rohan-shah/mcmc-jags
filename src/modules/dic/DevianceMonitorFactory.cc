#include "DevianceMonitorFactory.h"
#include "DevianceMonitor.h"

#include <graph/StochasticNode.h>

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

}
