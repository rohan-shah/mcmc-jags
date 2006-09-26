#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node*> const &parents)
  : Node(dim, parents)
{}
