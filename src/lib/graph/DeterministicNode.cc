#include <config.h>
#include <graph/DeterministicNode.h>

using std::vector;
using std::set;

namespace jags {

DeterministicNode::DeterministicNode(vector<unsigned int> const &dim, 
                                     vector<Node const *> const &parents)
    : Node(dim, parents), _fixed(true)
{
    vector<Node const*>::const_iterator p;
    
    //Add this node as a deterministic child of its parents
    //taking care to avoid repeats
    set<Node const*> pset;
    for (p = parents.begin(); p != parents.end(); ++p) {
	if (pset.insert(*p).second) {
	    //Note that we can do this because DeterministicNode is a
	    //friend class of Node, giving us direct access to
	    //p->_dtrm_children
	    (*p)->_dtrm_children->push_back(this);
	}
    }

    //Deterministic nodes are not fixed if any parents are not fixed
    for (p = parents.begin(); p != parents.end(); ++p)
    {
	if (!(*p)->isFixed()) {
	    _fixed =  false;
	    break;
	}
    }

    /* 
       Fixed deterministic nodes should be immediately initialized by
       calling deterministicSample. We can't do this here because
       deterministicSample is a virtual function. So we have to do it
       in whatever sub-class defines it.
    */
       
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
