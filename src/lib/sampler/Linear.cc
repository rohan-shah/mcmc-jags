#include <config.h>
#include <sampler/Linear.h>
#include <sampler/GraphView.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <graph/Node.h>

using std::vector;
using std::set;

namespace jags {

typedef bool (DeterministicNode::*NodeCheckFn) (GraphMarks const&, bool) const;

static bool isLink(DeterministicNode const *dnode)
{
    return dynamic_cast<LinkNode const*>(dnode) != 0;
}

bool checkLinear(GraphView const *gv, bool fixed, bool link)
{
    vector<StochasticNode const *> const &sn = gv->stochasticChildren();
    vector<DeterministicNode *> const &dn = gv->deterministicChildren();

    set<Node const*> ancestors;
    //Sampled nodes are trivial (fixed) linear functions of themselves
#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    //Workaround for Solaris libCstd
    for (vector<StochasticNode const *>::const_iterator p = gv->nodes().begin();
         p != gv->nodes().end(); ++p)
      {
        ancestors.insert(*p);
      }
#else
    ancestors.insert(gv->nodes().begin(), gv->nodes().end());
#endif
    
    for (unsigned int j = 0; j < dn.size(); ++j) {
	if (dn[j]->isClosed(ancestors, DNODE_LINEAR, fixed)) {
	    ancestors.insert(dn[j]);
	}
	else if (link && isLink(dn[j])) {
	    // A link function is allowed if no other deterministic
	    // nodes in the GraphView depend on it.
	    for (unsigned int k = j + 1; k < dn.size(); ++k) {
		if (dn[j]->deterministicChildren()->count(dn[k])) {
		    return false;
		}
	    }
	}
	else {
	    return false;
	}
    }
    
    return true;
}

bool checkScale(GraphView const *gv, bool fixed)
{
    vector<DeterministicNode *> const &dnodes = gv->deterministicChildren();
    
    set<Node const*> ancestors;
    //FIXME: valid for multiple nodes?
#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    //Workaround for Solaris libCstd
    for (vector<StochasticNode const *>::const_iterator p = gv->nodes().begin();
         p != gv->nodes().end(); ++p)
      {
        ancestors.insert(*p);
      }
#else
    ancestors.insert(gv->nodes().begin(), gv->nodes().end());
#endif 

    //Start off looking for scale transformations, then fall back on
    //scale mixture transformations if fixed is false.

    bool mix = false;
    for (unsigned int j = 0; j < dnodes.size(); ++j) {

	if (!mix) {
	    if (dnodes[j]->isClosed(ancestors, DNODE_SCALE, fixed)) {
		ancestors.insert(dnodes[j]);
	    }
	    else if (!fixed) {
		mix = true;
	    }
	    else {
		return false;
	    }
	}

	if (mix) {
	    if (dnodes[j]->isClosed(ancestors, DNODE_SCALE_MIX, false)) {
		ancestors.insert(dnodes[j]);
	    }
	    else {
		return false;
	    }
	}
    }
    
    return true;
}

bool checkPower(GraphView const *gv, bool fixed)
{
    set<Node const*> ancestors;

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    //Workaround for Solaris libCstd
    for (vector<StochasticNode const *>::const_iterator p = gv->nodes().begin();
         p != gv->nodes().end(); ++p)
      {
        ancestors.insert(*p);
      }
#else
    ancestors.insert(gv->nodes().begin(), gv->nodes().end());
#endif 

    vector<DeterministicNode *> const &dnodes = gv->deterministicChildren();    
    for (unsigned int j = 0; j < dnodes.size(); ++j) {
	if (dnodes[j]->isClosed(ancestors, DNODE_POWER, fixed)) {
	    ancestors.insert(dnodes[j]);
	}
	else {
	    return false;
	}
    }
    
    return true;
}

} //namespace jags
