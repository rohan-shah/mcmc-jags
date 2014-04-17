#include <config.h>
#include "LDAFactory.h"
#include "LDA.h"

#include <sampler/MutableSampler.h>
#include <sampler/MutableSampleMethod.h>
#include <sampler/SingletonGraphView.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <graph/MixTab.h>
#include <distribution/Distribution.h>

#include <graph/VectorStochasticNode.h>

#include <set>
#include <map>
#include <algorithm>

using std::set;
using std::vector;
using std::string;
using std::map;
using std::list;
using std::find;

namespace jags {

    /* Struct to hold Dirichlet nodes that will be marginalized out
       by the LDA sampler */
    struct DirichletPriors { 
	vector<StochasticNode*> words;
	vector<StochasticNode*> topics;
    };
	
    typedef map<MixTab const *, DirichletPriors> LDAMap;

    static inline bool isCat(StochasticNode const *snode) {
	return snode->distribution()->name() == "dcat";
    }

    MixTab const *checkTopicPrior(GraphView const &gv, Graph const &graph)
    {
	/*
	  Dirichlet node TopicPrior has categorical stochastic
	  children.  There are no intermediate deterministic children.
	*/
	if (!gv.deterministicChildren().empty()) return 0;
	vector<StochasticNode *> const &schild = gv.stochasticChildren();
	for (unsigned int i = 0; i < schild.size(); ++i) {
	    if (!isCat(schild[i])) return 0;
	}

	/*
	  Each stochastic child acts as the index of a single mixture
	  node.  This mixture node has a single stochastic child with
	  a categorical distribution.
	  
	  All the mixture nodes have a common MixTab.
	*/
	  
	MixTab const *mtab = 0; 
	for (unsigned int i = 0; i < schild.size(); ++i) {

	    SingletonGraphView gvi(schild[i], graph);

	    vector<StochasticNode *> const &si = gvi.stochasticChildren();
	    if (si.size() != 1) return 0;
	    if (!isCat(si[0])) return 0;
	    
	    vector<DeterministicNode *> const &di = gvi.deterministicChildren();
	    if (di.size() != 1) return 0;
	    MixtureNode const *m = asMixture(di[0]);
	    if (m == 0) return 0;
	    
	    //Check that schild[i] is the index of the mixture node
	    if (m->index_size() != 1) return 0;
	    if (m->parents()[0] != schild[i]) return 0;
	    for (unsigned int j = 1; j < m->parents().size(); ++j) {
		if (m->parents()[j] == schild[i]) return 0;
	    }

	    if (i == 0) {
		mtab = m->mixTab();
	    }
	    else {
		if (m->mixTab() != mtab) return 0; 
	    }
	}
	return mtab;
    }

    MixTab const *checkWordPrior(GraphView const &gv, Graph const &graph)
    {
	/*
	   Dirichlet node WordPrior is related to multiple categorical
	   outcomes via a set of mixture nodes that all share the same
	   mixTable.
	*/
	vector<StochasticNode *> const &schild = gv.stochasticChildren();
	for (unsigned int i = 0; i < schild.size(); ++i) {
	    if (!isCat(schild[i])) return 0;
	}
	
	MixTab const *mtab = 0;
	vector<DeterministicNode *> const &dchild = gv.deterministicChildren();
	for (unsigned int j = 0; j < dchild.size(); ++j) {
	    
	    MixtureNode const *m = asMixture(dchild[j]);
	    if (m == 0) return 0;
	    
	    if (j == 0) {
		mtab = m->mixTab();
	    }
	    else if (mtab != m->mixTab()) {
		return 0;
	    }
	}
	
	return mtab;
    }

    namespace mix {
	
	Sampler * 
	LDAFactory::makeSampler(vector<StochasticNode*> const &topicPriors,
				vector<StochasticNode*> const &wordPriors,
				list<StochasticNode*> const &free_nodes,
				Graph const &graph) const
	{
	    if (topicPriors.empty() || wordPriors.empty()) return 0;

	    unsigned int nDoc = topicPriors.size();
	    vector<vector<StochasticNode*> > topics(nDoc), words(nDoc);
	    vector<StochasticNode*> snodes;
	    for (unsigned int d = 0; d < nDoc; ++d) {
		SingletonGraphView gvd(topicPriors[d], graph);
		topics[d] = gvd.stochasticChildren();
		for (unsigned int i = 0; i < topics[d].size(); ++i) {
		    if (find(free_nodes.begin(), free_nodes.end(), topics[d][i])
			== free_nodes.end()) 
		    {
			return 0;
		    }
		    SingletonGraphView gvi(topics[d][i], graph);
		    words[d].push_back(gvi.stochasticChildren()[0]);
		    snodes.push_back(topics[d][i]);
		}
	    }

	    if (LDA::canSample(topics, words, topicPriors, wordPriors, graph)) {
		
		GraphView *view = new GraphView(snodes, graph);
		unsigned int N = nchain(view);
		vector<MutableSampleMethod*> methods(N);
		for (unsigned int ch = 0; ch < N; ++ch) {
		    methods[ch] = new LDA(topics, words, topicPriors,
					  wordPriors, view, ch);
		}
		return new MutableSampler(view, methods, "mix::LDA");
	    }
	    else return 0;
	}

	string LDAFactory::name() const
	{
	    return "mix::LDA";
	}

	vector<Sampler*>  
	LDAFactory::makeSamplers(list<StochasticNode*> const &free_nodes, 
				 Graph const &graph) const
	{
	    //First we need to traverse the graph looking for
	    //Dirichlet nodes.  We are not interested in sampling
	    //them, but they are the basis for finding the categorical
	    //nodes that we do want to sample
	    
	    set<StochasticNode*> dirichlet_nodes;

	    for (Graph::const_iterator p = graph.begin(); p != graph.end(); ++p)
	    {
		VectorStochasticNode *vsnode = 
		    dynamic_cast<VectorStochasticNode *>(*p);
		if (vsnode && vsnode->distribution()->name() == "ddirch") {
	    dirichlet_nodes.insert(vsnode);
		}
	    }

	    // Now classify them according to their MixTab

	    LDAMap dirichlet_map;
	    for (set<StochasticNode*>::iterator p = dirichlet_nodes.begin();
		 p != dirichlet_nodes.end(); ++p)
	    {
		SingletonGraphView gv(*p, graph);
		if (MixTab const *mtab = checkWordPrior(gv, graph)) {
		    dirichlet_map[mtab].words.push_back(*p);
		}
		else if (MixTab const *mtab = checkTopicPrior(gv, graph)) {
		    dirichlet_map[mtab].topics.push_back(*p);
		}
	    }

	    // Traverse the elements of the mixtable
	    vector<Sampler*> samplers;
	    for (LDAMap::const_iterator p = dirichlet_map.begin(); 
		 p != dirichlet_map.end(); ++p)
	    {
		Sampler *s = makeSampler(p->second.topics, p->second.words,
					 free_nodes, graph);
					 
		if (s) samplers.push_back(s);
	    }
	    return samplers;
	}
    }
}
