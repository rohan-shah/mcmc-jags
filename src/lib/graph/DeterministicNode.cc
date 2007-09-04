#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node const *> const &parents)
  : Node(dim, parents)
{}

void DeterministicNode::randomSample(RNG*, unsigned int nchain) {
    deterministicSample(nchain);
}

bool DeterministicNode::isRandomVariable() const { 
    return false; 
}
