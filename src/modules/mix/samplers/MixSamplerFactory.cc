#include <config.h>


MixSamplerFactory::MixSamplerFactory()
{}

void
MixSamplerFactory::makeSampler(std::set<StochasticNode*> &nodes, 
			       Graph const &graph,
			       std::vector<std::vector<Sampler*> > &samplers) 
    const
{

    /* Find Stochastic Nodes in the graph with distribution DNormMix
       and add them to the set "mix_nodes" */

    set<StochasticNode*> mix_nodes;
    set<Node*>::const_iterator p;
    for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	StochasticNode const *snode = asStochastic(*p);
	if (snode && snode->distribution()->name() == "DNormMix") {
	    mix_nodes.insert(snode);
	}
    }
    
    for (set<StochasticNode*>::iterator i = mix_nodes.begin(); 
	 i != mix_nodes.end(); ++i)
    {
	StochasticNode *snode = *i;
	
	GraphMarks marks(graph);
	marks.markAncestors(snode,2);
	vector<StochasticNode *> sample_nodes;
	for (p = graph.nodes().begin(); p != graph.nodes().end(); ++p) {
	    if (marks.mark(*p) == 2) {
		StochasticNode *sample_node_p = asStochastic(*p);
		if (sample_node_p) {
		    sample_nodes.push_back(sample_node_p);
		}
	    }
	}

	
	
	
    }
}
