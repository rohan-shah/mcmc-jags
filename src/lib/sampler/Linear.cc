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

typedef bool (DeterministicNode::*NodeCheckFn) (GraphMarks const&, bool) const;

static bool isLink(DeterministicNode const *dnode)
{
    return dynamic_cast<LinkNode const*>(dnode) != 0;
}

bool checkLinear(GraphView const *gv, bool fixed, bool link)
{
    vector<StochasticNode const *> const &stoch_nodes
	= gv->stochasticChildren();
    vector<DeterministicNode *> const &dtrm_nodes
	= gv->deterministicChildren();

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
    
    set<Node const*> stoch_node_parents;
    if (link) {
	//Put parents of stoch_nodes, which may be link functions
	for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	    stoch_node_parents.insert(stoch_nodes[i]->parents().begin(),
				      stoch_nodes[i]->parents().end());
	}
    }
    
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	if (dtrm_nodes[j]->isClosed(ancestors, DNODE_LINEAR, fixed)) {
	    ancestors.insert(dtrm_nodes[j]);
	}
	else if (link) {
	    //Allow an exception for link nodes
	    if (stoch_node_parents.count(dtrm_nodes[j]) == 0 || 
		!isLink(dtrm_nodes[j]))
	    {
		return false;
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
