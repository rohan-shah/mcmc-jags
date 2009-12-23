#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node const *> const &parents)
    : Node(dim, parents), _observed(true)
{
    //Add this node as a deterministic child of its parents
    for (unsigned int i = 0; i < parents.size(); ++i) {
	parents[i]->addChild(this);
    }

    //Deterministic nodes are not observed if any parents are unobserved
    vector<Node const*>::const_iterator p;
    for (p = parents.begin(); p != parents.end(); ++p)
    {
	if (!(*p)->isObserved()) {
	    _observed =  false;
	    break;
	}
    }

    /* 
       Observed deterministic nodes should be immediately initialized
       by calling deterministicSample. We can't do this here because
       this is a virtual function. So we have to do it in whatever
       sub-class defines the deterministicSample member function.
    */
       
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
    return _observed;
}

