#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;
using std::set;

namespace jags {

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node const *> const &parents)
    : Node(dim, parents), _fixed(true)
{
    //Add this node as a deterministic child of its parents
    //taking care to avoid repeats
    set<Node const*> pset;
    for (unsigned int i = 0; i < parents.size(); ++i) {
	Node const *p = parents[i];
	if (pset.insert(p).second) {
	    p->addChild(this);
	}
    }

    //Deterministic nodes are not fixed if any parents are not fixed
    vector<Node const*>::const_iterator p;
    for (p = parents.begin(); p != parents.end(); ++p)
    {
	if (!(*p)->isFixed()) {
	    _fixed =  false;
	    break;
	}
    }

    /* 
       Fixed deterministic nodes should be immediately initialized by
       calling deterministicSample. We can't do this here because this
       is a virtual function. So we have to do it in whatever
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

RVStatus DeterministicNode::randomVariableStatus() const 
{ 
    return RV_FALSE; 
}

bool DeterministicNode::isFixed() const
{
    return _fixed;
}

} //namespace jags
