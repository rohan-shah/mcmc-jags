#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node const *> const &parents)
  : Node(dim, parents)
{
    for (unsigned int i = 0; i < parents.size(); ++i) {
	parents[i]->addChild(this);
    }
}

DeterministicNode::~DeterministicNode()
{
    for (unsigned int i = 0; i < parents().size(); ++i) {
	parents()[i]->removeChild(this);
    }
}

void DeterministicNode::randomSample(RNG*, unsigned int nchain) {
    deterministicSample(nchain);
}

bool DeterministicNode::isRandomVariable() const { 
    return false; 
}

bool DeterministicNode::isObserved() const
{
    vector<Node const*>::const_iterator p;
    for (p = parents().begin(); p != parents().end(); ++p)
    {
	if (!(*p)->isObserved())
	    return false;
    }
    return true;
}

bool DeterministicNode::isPower(GraphMarks const &power_marks,
				bool fixed) const
{
    return isScale(power_marks, false);
}
