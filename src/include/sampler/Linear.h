#ifndef LINEAR_H_
#define LINEAR_H_

#include<vector>

class StochasticNode;
class Node;
class Graph;

/**
 * Helper function to check linearity. The function returns true if
 * all deterministic descendants of the given stochastic nodes (within
 * the given graph) are linear functions.
 *
 * @param snodes Vector of stochastic nodes.
 *
 * @param dtrm_nodes Vector of deterministic descendants of the nodes
 * in the argument snodes, within some (implicitly defined) graph.
 *
 * @pararm fixed Boolean flag. If true, the function checks for fixed
 * linear functions.
 *
 * @see Node#isLinear
 */
bool checkLinear(std::vector<StochasticNode *> const &snodes,
		 Graph const &graph, bool fixed);

/**
 * Helper function to check for scale transformations. The function
 * returns true if all deterministic descendants of the given node
 * (within the given graph) are scale transformations.
 *
 * @param snode Node whose descendants are to be checked.
 *
 * @param graph Deterministic descendents of snode in some (implicitly
 * defined) graph.
 *
 * @pararm fixed Boolean flag. If true, the function checks for fixed
 * scale transformations.
 *
 * @see Node#isScale
 */
bool checkScale(StochasticNode *snode, Graph const &graph, bool fixed);

#endif /* LINEAR_H_ */
