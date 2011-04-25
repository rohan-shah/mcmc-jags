#include <config.h>

#include "ConjugateDirichlet.h"

#include <rng/RNG.h>
#include <graph/MixtureNode.h>
#include <graph/StochasticNode.h>
#include <graph/AggNode.h>
#include <graph/NodeError.h>
#include <sampler/GraphView.h>
#include <module/ModuleError.h>

#include <set>
#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::set;
using std::string;

static bool checkAggNode(AggNode const *anode, 
			 set<Node const *> const &nodeset)
{
    /* 
       Utility function called by ConjugateDirichlet::canSample

       The aggregate node (anode) must have only one parent in
       nodeset.  This parent must be embedded entirely within anode
       (i.e. we cannot take a subset of it) and the order of the
       elements cannot be permuted.
    */

    vector<Node const *> const &par = anode->parents();
    vector<unsigned int> const &off = anode->offsets();

    //Find unique parent
    Node const *param = 0;
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (nodeset.count(par[i])) {
	    if (param) {
		if (par[i] != param) return false;
	    }
	    else {
		param = par[i];
	    }
	}
    }
    if (param == 0) {
	throwLogicError("Error 1 in ConjugateDirichlet::canSample");
    }

    //Check that parent is entirely contained in anode with offsets
    //in ascending order
    unsigned int j = 0;
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (par[i] == param) {
	    if (off[i] != j) return false;
	    ++j;
	}
    }
    if (j != param->length()) {
	return false;
    }

    return true;
}

static bool checkMixNode(MixtureNode const *mnode, 
			 set<Node const *> const &nodeset)
{
    /*
      Utility function called by ConjugateDirichlet::canSample
      
      None of the indices may depend on nodeset. Among the other
      parents, only 1 may be in nodeset (although it may appear
      several times)
    */

    vector<Node const *> const &par = mnode->parents();

    //Check indices
    unsigned int nindex = mnode->index_size();
    for (unsigned int i = 0; i < nindex; ++i) {
	if (nodeset.count(par[i]))
	    return false;
    }

    //Find unique parent
    Node const *param = 0;
    for (unsigned int i = nindex; i < par.size(); ++i) {
	if (nodeset.count(par[i])) {
	    if (param) {
		if (param != par[i]) return false;
	    }
	    else {
		param = par[i];
	    }
	}
    }
    if (param == 0) {
	throwLogicError("Error 2 in ConjugateDirichlet::canSample");
    }

    return true;
}

namespace bugs {

bool ConjugateDirichlet::canSample(StochasticNode *snode, Graph const &graph)
{
    /*
      The ConjugateDirichlet sampler aims to be as general as
      possible, while minimizing the calculations that have to be done
      at each iteration.
      
      The stochastic children may be multinomial or categorical, and the
      deterministic children may include mixture nodes and aggregate
      nodes. However there are some restrictions that are implemented in
      the checkAggNode and checkMixNode functions.
      
      1) The deterministic descendents must form a tree, so that there is
      only one pathway from the sample node (snode) to each stochastic child.
      2) Aggregate nodes can only be used to embed snode (or one of its
      determistic descendants) in a larger node, not to take a subset.
      3) When a node X is embedded in an aggregate node Y, the order of the
      elements of X cannot be permuted in Y.

      These restrictions ensure that the _off values used by the
      update method are the same across chains and across iterations, and
      can therefore be calculated once in the constructor.

      Fascinating fact: Neither ddirch nor dcat requires a normalized
      probability argument.  This allows us to embed snode inside a
      larger aggregate node without worrying about what the other
      values are. For example:
      
      Y ~ dcat(p)
      p[1:3] ~ ddirch(alpha)
      p[4:5] <- W

      We can completely ignore the values of W (p[4] and p[5]). Normally
      these will be zero but we don't require it.
    */
    
    if(getDist(snode) != DIRCH)
	return false;
    
    if (isBounded(snode))
	return false;
    
    GraphView gv(snode, graph);
    vector<DeterministicNode*> const &dchild = gv.deterministicChildren();
    vector<StochasticNode const*> const &schild = gv.stochasticChildren();
    
    // Check stochastic children
    for (unsigned int i = 0; i < schild.size(); ++i) {
	vector<Node const *> const &param = schild[i]->parents();
	if (isBounded(schild[i])) {
	    return false; //Truncated
	}
	switch(getDist(schild[i])) {
	case CAT:
	    break;
	case MULTI:
	    if (gv.isDependent(param[1]))
		return false;
	    break;
	default:
	    return false;
	}
    }

    // Check deterministic descendants
    set<Node const *> nodeset;
    nodeset.insert(snode);
    for (unsigned int j = 0; j < dchild.size(); ++j) {
	if (MixtureNode const *m = dynamic_cast<MixtureNode const*>(dchild[j]))
	{
	    if (!checkMixNode(m, nodeset)) return false;	    
	}
	else if (AggNode const *a = dynamic_cast<AggNode const*>(dchild[j]))
	{
	    if (!checkAggNode(a, nodeset)) return false;
	}
	else {
	    return false;
	}
	nodeset.insert(dchild[j]);
    }
  
    return true;
}


static AggNode const *
getAggParent(Node const *node, set<Node const *> const &nodeset)
{
    /* 
       Search recursively for an AggNode ancestor node within nodeset.
       We assume that node has either 1 parent in nodeset or 0.
    */

    vector<Node const*> const &par = node->parents();
    Node const *param = 0;
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (nodeset.count(par[i])) {
	    param = par[i];
	    break;
	}
    }
    if (param == 0) {
	return 0;
    }
    
    AggNode const *anode = dynamic_cast<AggNode const *>(param);
    if (anode == 0) {
	anode = getAggParent(param, nodeset);
    }
    return anode;
}

static vector<vector<unsigned int> > 
makeOffsets(GraphView const *gv, set<Node const *> const &nodeset) 
{
    vector<StochasticNode const *> const &schild = gv->stochasticChildren();
    unsigned int nchild = schild.size();
    vector<vector<unsigned int> > ans(nchild);

    for (unsigned int i = 0; i < nchild; ++i) {
	vector<unsigned int> off_i;
	if (AggNode const *anode = getAggParent(schild[i], nodeset)) {

	    vector<Node const *> const &par = anode->parents();
	    vector<unsigned int> const &off = anode->offsets();
	    
	    Node const *param = 0;
	    for (unsigned int j = 0; j < par.size(); ++j) {
		if (nodeset.count(par[j])) {
		    param = par[j];
		    break;
		}
	    }
	    if (param == 0) {
		throwLogicError("Error 3 in ConjugateDirichlet::canSample");
	    }

	    for (unsigned int j = 0; j < par.size(); ++j) {
		if (par[j] == param) {
		    off_i.push_back(off[j]);
		}
	    }
	    ans[i] = off_i;
	}
    }

    return ans;
}

static bool findMix(GraphView const *gv)
{
    //Find mixture nodes among deterministic descendants
    vector<DeterministicNode*> const &dchild = gv->deterministicChildren();
    for (unsigned int i = 0; i < dchild.size(); ++i) {
	if (isMixture(dchild[i])) return true;
    }
    return false;
}


ConjugateDirichlet::ConjugateDirichlet(GraphView const *gv)
    : ConjugateMethod(gv), _mix(findMix(gv)), 
      _off(gv->stochasticChildren().size())
{
    //Find out if we have any aggregate nodes in the deterministic
    //children
    vector<DeterministicNode *> dchild = gv->deterministicChildren();
    bool have_agg = false;
    for (unsigned int i = 0; i < dchild.size(); ++i) {
	if (dynamic_cast<AggNode const *>(dchild[i])) {
	    have_agg = true;
	    break;
	}
    }
    if (have_agg) {
	//We have aggregate nodes and need to set up offsets for the
	//update function
	set<Node const*> nodeset;
	nodeset.insert(gv->nodes()[0]);
	for (unsigned int i = 0; i < dchild.size(); ++i) {
	    nodeset.insert(dchild[i]);
	}
	_off = makeOffsets(gv, nodeset);
	
	//Check offsets are the right length
	unsigned int slength = gv->nodes()[0]->length();
	for (unsigned int i = 0; i < _off.size(); ++i) {
	    if (!_off[i].empty() && _off[i].size() != slength) {
		throwLogicError("Invalid offsets if ConjugateDirichlet");
	    }
	}
    }
}

static bool checkzero(StochasticNode const *snode, unsigned int chain,
		      vector<unsigned int> const &offsets)
{
    double const *par = snode->parents()[0]->value(chain);
    unsigned int length = snode->parents()[0]->length();

    if (offsets.empty()) {
	for (unsigned int i = 0; i < length; ++i) {
	    if (par[i] != 0) return false;
	}
    }
    else {
	for (unsigned int i = 0; i < offsets.size(); ++i) {
	    if (par[offsets[i]] != 0) return false;
	}
    }
    return true;
}

void ConjugateDirichlet::update(unsigned int chain, RNG *rng) const
{
    StochasticNode *snode = _gv->nodes()[0];
    unsigned long size = snode->length();
    double *alpha = new double[size];
    double *xnew = new double[size];

    double const *prior = snode->parents()[0]->value(chain);
    for (unsigned long i = 0; i < size; ++i) {
	alpha[i] = prior[i];
    }

    vector<StochasticNode const*> const &schild = _gv->stochasticChildren();
    unsigned int nchildren = schild.size();
    
    if (_mix) {
	//Set all elements of snode to zero. This is an illegal value
	//used to find stochastic children that are "active" (i.e. make
	//a likelihood contribution in a mixture model.
	for (unsigned int i = 0; i < size; ++i) {
	    xnew[i] = 0;
	}
	_gv->setValue(xnew, size, chain);
    }

    for (unsigned int i = 0; i < nchildren; ++i) {
	int index = 0;
	double const *N = 0;
	if (!_mix || checkzero(schild[i], chain, _off[i])) {
	    switch(_child_dist[i]) {
	    case MULTI:
		N = schild[i]->value(chain);
		if (_off[i].empty()) {
		    for (unsigned int j = 0; j < size; ++j) {
			alpha[j] += N[j];
		    }
		}
		else {
		    for (unsigned int j = 0; j < size; ++j) {
			alpha[j] += N[_off[i][j]];
		    }
		}
		break;
	    case CAT:
		index = static_cast<int>(*schild[i]->value(chain)) - 1;
		if (_off[i].empty()) {
		    alpha[index] += 1;
		}
		else {
		    for (unsigned int j = 0; j < size; ++j) {
			if (index == _off[i][j]) {
			    alpha[j] += 1;
			    break;
			}
		    }
		}
		break;
	    default:
		throwLogicError("Invalid distribution in ConjugateDirichlet");
	    }
	}
    }

    /* Check structural zeros */
    for (unsigned int i = 0; i < size; ++i) {
	if (prior[i] == 0 && alpha[i] != 0) {
	    throwNodeError(snode, "Invalid likelihood for Dirichlet distribution with structural zeros");
	}
    }
  
    /* 
       Draw Dirichlet sample by drawing independent gamma random
       variates and then normalizing
    */

    double xsum = 0.0;
    for (unsigned long i = 0; i < size; ++i) {
	if (alpha[i] > 0) {
	    xnew[i] = rgamma(alpha[i], 1, rng);
	    xsum += xnew[i];
	}
	else {
	    xnew[i] = 0;
	}
    }
    for (unsigned long i = 0; i < size; ++i) {
	xnew[i] /= xsum;
    }

    _gv->setValue(xnew, size, chain);

    delete [] xnew;
    delete [] alpha;
}

string ConjugateDirichlet::name() const
{
    return "ConjugateDirichlet";
}

}
