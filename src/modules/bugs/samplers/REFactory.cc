#include <config.h>
#include <distribution/Distribution.h>

#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>
#include <sarray/nainf.h>

#include "REFactory.h"
#include "RESampler.h"

#include <cmath>

using std::set;
using std::vector;
using std::log;
using std::string;

/* Smallest number of random effects that will be sampled */
#define N_MIN 20

#include <graph/StochasticNode.h>
static bool is_stoch(Node const *node)
{
   return asStochastic(node);
}

namespace jags {
namespace bugs {

REFactory::REFactory()
    : _name("RE")
{}

void REFactory::makeSampler(set<StochasticNode*, less_sampler> &nodes,
			    Graph const &graph,
			    vector<vector<Sampler*> > &samplers) const
{

    set<StochasticNode*, less_sampler>::const_iterator p;
    vector<StochasticNode*> effect_nodes;
    vector<StochasticNode*> variance_nodes;
    GraphMarks marks(graph);
    set<Node*>::const_iterator q;

    for (p = nodes.begin(); p != nodes.end(); ++p) {
	//Find unbounded Normal nodes with mean zero and unobserved precision
	if ((*p)->distribution()->name() == "dnorm" && !(*p)->isBounded()) {
	    Node const *mu = (*p)->parents()[0];
	    Node const *tau = (*p)->parents()[1];
	    if (mu->isFixed() && mu->value(0)[0] == 0 && !tau->isFixed())
            {

		/* The precision parameter (tau) should be a function
		   of a single unobserved, scalar, stochastic node
		   (and possibly multiple observed nodes)
		*/
		marks.markAll(0);
		marks.markParents(*p, is_stoch, 1);
		
		StochasticNode *parent = 0;
		bool cansample = false;
		for (q = graph.nodes().begin(); q != graph.nodes().end(); ++q) 
		{
		    if (marks.mark(*q) == 1 && !(*q)->isFixed()) {
			if (parent) {
			    // Too many parents - give up
			    cansample = false;
			    break;
			}
			else if ((*q)->length() != 1) {
			    cansample = false;
			    break;
			}
			else {
			    parent = dynamic_cast<StochasticNode*>(*q);
			    cansample = true;
			}
		    }
		}
		if (cansample) {

		    //Check support of parent node is fixed
		    if (isSupportFixed(parent)) {
			effect_nodes.push_back(*p);
			variance_nodes.push_back(parent);
		    }
		}
	    }
	}
    }

    set<Node*> eff;
    set<StochasticNode*> var;
    for (unsigned int i = 0; i < effect_nodes.size(); ++i) {
	eff.insert(effect_nodes[i]);
	var.insert(variance_nodes[i]);
    }

    for (p = var.begin(); p != var.end(); ++p) {

	marks.markAll(0);
	marks.markChildren(*p, is_stoch, 1);
	bool cansample = true;
	vector<StochasticNode*> effects;
	for (q = graph.nodes().begin(); q != graph.nodes().end(); ++q) {
	    if (marks.mark(*q) == 1 && eff.count(*q) == 0) {
		/* The variance component has a stochastic child
		   that is not one of the random effects */
		cansample = false;
		break;
		
	    }
	}
	if (cansample) {
	    vector<StochasticNode*> effects;
	    for (unsigned int i = 0; i < variance_nodes.size(); ++i) {
		if (variance_nodes[i] == *p) {
		    effects.push_back(effect_nodes[i]);
		}
	    }
	    unsigned int N = effects.size();
	    if (N >= N_MIN) {

		nodes.erase(*p);
		for (unsigned int j = 0; j < effects.size(); ++j) {
		    nodes.erase(effects[j]);
		}
		
		double *initial_value = new double[N + 1];
		double lower, upper;
		for (unsigned int ch = 0; ch < samplers.size(); ++ch) {

		    double v = (*p)->value(ch)[0];
		    support(&lower, &upper, 1U, *p, ch);
		    bool bb = jags_finite(lower);
		    bool ba = jags_finite(upper);
		    if (bb && ba) {
			initial_value[0] = log(v - lower) - log(upper - v);
		    }
		    else if (bb) {
			initial_value[0] = log(v - lower);
		    }
		    else if (ba) {
			initial_value[0] = log(upper - v);
		    }
		    else {
			initial_value[0] = v;
		    }
		    for (unsigned int k = 1; k < N; ++k) {
			initial_value[k] = *effects[k-1]->value(ch);
		    }
		    Sampler *sampler = 
			new RESampler(*p, effects, graph, ch,
				      initial_value, N + 1);
		    samplers[ch].push_back(sampler);
		}
		delete [] initial_value;
	    }
	}
    }
}

string const &REFactory::name() const
{
    return _name;
}

}}
