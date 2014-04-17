#include <config.h>
#include "DirichletCatFactory.h"
#include "DirichletCat.h"

#include <sampler/MutableSampler.h>
#include <sampler/MutableSampleMethod.h>
#include <sampler/SingletonGraphView.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <distribution/Distribution.h>

#include <set>
#include <map>

using std::list;
using std::set;
using std::vector;
using std::string;
using std::map;

#define NLEVEL 200
#define MAX_TEMP 100
#define NREP 5

namespace jags {

    //STL syntax becomes unreadable if we don't use a typedef
    typedef map<vector<StochasticNode *>, vector<StochasticNode*> > DCMap;

    static
    bool isCandidate(SingletonGraphView const &gv)
    {
	vector<StochasticNode *> const &schild = gv.stochasticChildren();
	vector<DeterministicNode *> const &dchild = gv.deterministicChildren();

	//A necessary but not sufficient condition: we do this first
	//because it is fast
	if (schild.size() != dchild.size()) return false;
	
	//Stochastic children must all have dcat distribution
	Distribution const *dist0 = schild[0]->distribution();
	if (dist0->name() != "dcat") return false;
	for (unsigned int i = 1; i < schild.size(); ++i) {
	    if (schild[i]->distribution() != dist0) return false;
	}
	
	 //Deterministic descendants must all be mixture nodes
	 for (unsigned int j = 0; j < dchild.size(); ++j) {
	     if (!isMixture(dchild[j])) return false;
	 }

	 return true;
     }

     namespace mix {

	 Sampler * 
	 DirichletCatFactory::makeSampler(vector<StochasticNode*> const &snodes, 
					  Graph const &graph) const
	 {
	     GraphView *gv = new GraphView(snodes, graph);
	     Sampler * sampler = 0;
	     unsigned int nchain = snodes[0]->nchain();

	     if (DirichletCat::canSample(gv)) {
		 vector<MutableSampleMethod*> methods(nchain);	    
		 for (unsigned int ch = 0; ch < nchain; ++ch) {
		     methods[ch] = new DirichletCat(gv, ch);
		 }
		 sampler = new MutableSampler(gv, methods, "mix::DirichletCat");		
	     }
	     else {
		 delete gv;
	     }
	     return sampler;
	 }

	 string DirichletCatFactory::name() const
	 {
	     return "mix::DirichletCat";
	 }


	vector<Sampler*>  
	DirichletCatFactory::makeSamplers(list<StochasticNode*> const &nodes, 
					  Graph const &graph) const
	{
	    //Assemble candidates from available nodes and classify
	    //them by their stochastic children
	    DCMap cmap;

	    for (list<StochasticNode*>::const_iterator p = nodes.begin();
		 p != nodes.end(); ++p)
	    {
		if ((*p)->distribution()->name() != "ddirch") continue;
		SingletonGraphView gv(*p, graph);
		if (isCandidate(gv)) {
		    cmap[gv.stochasticChildren()].push_back(*p);
		}
	    }
	    
	    //Now traverse the candidate map and generate samplers
	    vector<Sampler*> samplers;
	    for (DCMap::const_iterator q = cmap.begin(); q != cmap.end(); ++q)
	    {
		Sampler *s = makeSampler(q->second, graph);		
		if (s) samplers.push_back(s);
	    }

	    return samplers;
	}

    }
}
