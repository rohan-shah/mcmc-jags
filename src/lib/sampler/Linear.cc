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

    set<Node const *> parameters;
    Graph pgraph;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	parameters.insert(snodes[i]);
        pgraph.add(snodes[i]);
    }

    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isLinear(parameters, pgraph, fixed)) {	
	    parameters.insert(dtrm_nodes[j]);
            pgraph.add(dtrm_nodes[j]);
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

    set<Node const *> parameters;
    parameters.insert(snode);
    Graph pgraph;
    pgraph.add(snode);

    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isScale(parameters, pgraph, fixed)) {	
	    parameters.insert(dtrm_nodes[j]);
            pgraph.add(dtrm_nodes[j]);
	}
	else {
	    return false;
	}
    }

    return true;
}
