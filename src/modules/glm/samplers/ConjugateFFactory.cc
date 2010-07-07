#include <config.h>

#include "ConjugateFFactory.h"
#include "ConjugateFSampler.h"
#include "ConjugateFMethod.h"

#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <distribution/Distribution.h>
#include <sampler/GraphView.h>
#include <sampler/Linear.h>

#include <stdexcept>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::set;
using std::logic_error;

static void getStochasticChildren(Node *node, set<StochasticNode*> &sset)
{
    //Find all stochastic descendants of the given node and add them to the
    //given set

    set<StochasticNode*> const *schildren = node->stochasticChildren();
    sset.insert(schildren->begin(), schildren->end());

    set<DeterministicNode*> const *dchildren = node->deterministicChildren();
    set<DeterministicNode*>::const_iterator p;
    for (p = dchildren->begin(); p != dchildren->end(); ++p) {
	getStochasticChildren(*p, sset);
    }
}

static void convertStochasticChildren(StochasticNode *snode,
				      vector<StochasticNode const*> const &in,
				      vector<StochasticNode *> &out)
{
    /* 
       We need this function to convert the stochastic children from
       const pointers to non-const pointers.

       FIXME: This might prompt a review of the GraphView class
    */
    set<StochasticNode*> sset;
    getStochasticChildren(snode, sset);

    out.clear();
    for (unsigned int i = 0; i < in.size(); ++i) {
	Node const *cnode = in[i];
	set<StochasticNode*>::const_iterator p = sset.begin();
	while (p != sset.end())
	{
	    if (cnode == *p) {
		out.push_back(*p);
		break;
	    }
	    else {
		++p;
	    }
	}
	if (p == sset.end()) {
	    throw logic_error("Conversion failure in Conjugate F method");
	}
    }
}


bool 
ConjugateFFactory::canSample(StochasticNode *snode, Graph const &graph) const
{
    /* 
       The target node has an F(m,1) prior, where m is fixed. It cannot
       be bounded
    */
    if (snode->distribution()->name() != "df") 
	return false;
    if (!snode->parents()[0]->isObserved())
	return false; //FIXME. Is this necessary?
    if (!snode->parents()[1]->isObserved())
	return false;
    if (*snode->parents()[1]->value(0) != 1)
	return false;
    if (isBounded(snode))
	return false; //FIXME. We could include this case

    /* 
       The stochastic children of the sampled node must be unobserved,
       normally distributed random variables, such that the precision
       is a scale function of the sampled node and the mean is independent
       of the sampled node
    */

    GraphView gv1(vector<StochasticNode*>(1,snode), graph);

    // Check stochastic children
    vector<StochasticNode const*> const &schildren1 = gv1.stochasticChildren();
    for (unsigned int i = 0; i < schildren1.size(); ++i) {
	if (schildren1[i]->distribution()->name() != "dnorm") {
	    return false;
	}
	if (gv1.isDependent(schildren1[i]->parents()[0])) {
	    return false; //mean parameter depends on snode
	}
	if (isBounded(schildren1[i])) {
	    return false;
	}
	if (schildren1[i]->isObserved()) {
	    return false;
	}
    }

    // Check that deterministic descendants are scale transformations 
    if (!checkScale(&gv1, false)) {
	return false;
    }

    // Need to get non-const pointers to stochastic children
    vector<StochasticNode*> snodes;
    convertStochasticChildren(snode, schildren1, snodes);
    
    /* The stochastic children themselves must form a linear model */

    GraphView gv2(snodes, graph);
    if (!checkLinear(&gv2, false, false))
	return false;

    vector<StochasticNode const *> const &schildren2 = gv2.stochasticChildren();
    for (unsigned int i = 0; i < schildren2.size(); ++i) {
	if (schildren2[i]->distribution()->name() != "dnorm") {
	    return false;
	}
	if (isBounded(schildren2[i])) {
	    return false;
	}
	if (gv2.isDependent(schildren2[i]->parents()[1])) {
	    return false; //Precision parameter is dependent
	}
    }

    return true;
}

Sampler *ConjugateFFactory::makeSampler(StochasticNode *snode1, 
					Graph const &graph) const
{
    unsigned int nchain = snode1->nchain();
    vector<ConjugateFMethod*> methods(nchain, 0);

    GraphView *gv1 = new GraphView(snode1, graph);

    vector<StochasticNode*> snodes2;
    convertStochasticChildren(snode1, gv1->stochasticChildren(), snodes2);
    GraphView *gv2 = new GraphView(snodes2, graph);

    for (unsigned int ch = 0; ch < nchain; ++ch) {
	methods[ch] = new ConjugateFMethod(gv1, gv2, ch);
    }
    
    return new ConjugateFSampler(gv1, gv2, methods);
}

string ConjugateFFactory::name() const
{
    return "glm::ConjugateF";
}
