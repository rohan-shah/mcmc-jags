#include <config.h>

#include "ConjugateFFactory.h"
#include "ConjugateFSampler.h"
#include "ConjugateFMethod.h"

#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <distribution/Distribution.h>
#include <sampler/GraphView.h>
#include <sampler/Linear.h>
#include <module/ModuleError.h>

#include <string>
#include <algorithm>

using std::vector;
using std::string;
using std::set;
using std::copy;
using std::stable_sort;

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

static bool lt_snodeptr (StochasticNode const **arg1, 
			 StochasticNode const **arg2) 
{
    return *arg1 < *arg2;
}

static void convertStochasticChildren(StochasticNode *snode,
				      vector<StochasticNode const*> const &in,
				      vector<StochasticNode *> &out)
{
    /* 
       We need this function to convert the stochastic children from
       const pointers to non-const pointers in the same order.

       FIXME: This might prompt a review of the GraphView class
    */
    set<StochasticNode*> sset;
    getStochasticChildren(snode, sset);

    /* 
       It is much more efficient to work with a sorted vector. But we
       need to keep track of the original ordering so we can assign
       the elements of the out vector in the same order as the in
       vector.
    */
    unsigned int N = in.size();
    StochasticNode const **inarray = new StochasticNode const *[N];
    copy(in.begin(), in.end(), inarray);
    StochasticNode const ***inptrs = new StochasticNode const **[N];
    for (unsigned int i = 0; i < N; ++i) {
	inptrs[i] = inarray + i;
    }
    stable_sort(inptrs, inptrs + N, lt_snodeptr);

    out.resize(N);
    bool ok = true;
    set<StochasticNode*>::const_iterator p = sset.begin();
    for (unsigned int i = 0; i < N; ++i) {
	Node const *cnode = *inptrs[i];
	while (cnode > *p) {
	    ++p;
	}
	if (cnode == *p) {
	    out[inptrs[i] - inarray] = *p;
	}
	else {
	    ok = false;
	    break;
	}
    }
    
    delete [] inarray;
    delete [] inptrs;

    if (!ok) {
	throwLogicError("Conversion mismatch in Conjugate F Factory");
    }
}

namespace glm {

  ConjugateFFactory::~ConjugateFFactory()
  {
    //Nothing to do here; only for vtable. BUT WHY???
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
  
}
