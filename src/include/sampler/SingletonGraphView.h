#ifndef SINGLETON_GRAPH_VIEW_H_ 
#define SINGLETON_GRAPH_VIEW_H_

#include <sampler/GraphView.h>

namespace jags {

    class StochasticNode;
    class Graph;

    /**
     * @short GraphView for a single stochastic node.
     *
     * The SingletonGraphView class provides a simplified interface to
     * the GraphView class there is only a single node to be sampled.
     * It should be used in preference to GraphView by any SampleMethod
     * that acts on a single node.
     */
    class SingletonGraphView : public GraphView {
      public:
	/**
	 * Constructs a SingletonGraphView for the given node.  
	 *
	 * @param node Node to be sampled 
	 *
	 * @param graph Graph within which sampling is to take place. 
	 */
        SingletonGraphView(StochasticNode * node, Graph const &graph)
	    : GraphView(std::vector<StochasticNode *>(1, node), graph, false) 
	    {} 
	/**
	 * Returns the sampled node.
	 */
	inline StochasticNode * node() const
	{
	    return nodes()[0];
	}
    };
    
} /* namespace jags */

#endif /* SINGLETON_GRAPH_VIEW_H_ */
