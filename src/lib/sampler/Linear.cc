#include <config.h>
#include <sampler/Linear.h>
#include <sampler/Sampler.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/Node.h>

using std::vector;
using std::set;

bool checkLinear(vector<StochasticNode*> const &snodes, Graph const &graph,
		 bool fixed)
{
    vector<StochasticNode const *> stoch_nodes;
    vector<Node *> dtrm_nodes;
    Sampler::classifyChildren(snodes, graph, stoch_nodes, dtrm_nodes);

    GraphMarks linear_marks(graph);
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	//Stochastic nodes are trivial linear functions of themselves
	linear_marks.mark(snodes[i], MARK_TRUE);
    }
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isLinear(linear_marks, fixed)) {	
	    linear_marks.mark(dtrm_nodes[j], MARK_TRUE);
	}
	else {
	    return false;
	}
    }

    return true;
}

bool checkScale(StochasticNode* snode, Graph const &graph, bool fixed)
{
    vector<StochasticNode const *> stoch_nodes;
    vector<Node *> dtrm_nodes;
    Sampler::classifyChildren(vector<StochasticNode *>(1, snode), graph, 
			      stoch_nodes, dtrm_nodes);

    GraphMarks scale_marks(graph);
    scale_marks.mark(snode, MARK_TRUE);

    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isScale(scale_marks, fixed)) {	
	    scale_marks.mark(dtrm_nodes[j], MARK_TRUE);
	}
	else {
	    return false;
	}
    }

    return true;
}
