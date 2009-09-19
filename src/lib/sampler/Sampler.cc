#include <config.h>
#include <sampler/Sampler.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>
#include <util/nainf.h>

#include <stdexcept>
#include <set>
#include <string>
#include <cmath>
#include <algorithm>

using std::vector;
using std::set;
using std::runtime_error;
using std::logic_error;
using std::string;
using std::reverse;

static bool isInformative(StochasticNode *node, Graph const &sample_graph);
static bool isInformative(DeterministicNode *node, Graph const &sample_graph);

static unsigned int sumLength(vector<StochasticNode *> const &nodes)
{
    //Adds up the length of a vector of stochastic nodes
    unsigned int n = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	n += nodes[i]->length();
    }
    return n;
}

Sampler::Sampler(vector<StochasticNode *> const &nodes, Graph const &graph)
    : _length(sumLength(nodes)), _nodes(nodes), _stoch_children(0),
      _determ_children(0)
{
    classifyChildren(nodes, graph, _stoch_children, _determ_children);
}

Sampler::~Sampler()
{
  // virtual destructor
}

vector<StochasticNode *> const &Sampler::nodes() const
{
  return _nodes;
}

/* A node is informative if it has an observed descendant. There are two
   functions to check informativeness: one for stochastic nodes and one
   for deterministic nodes.  The only difference is that for stochastic nodes
   we check to see if the node is observed.
*/
static bool isInformative(StochasticNode *node, Graph const &sample_graph)
{
    if (!sample_graph.contains(node))
	return false;

    if (node->isObserved())
	return true;

    for (StochasticNodeSet::iterator p = node->stochasticChildren()->begin();
         p != node->stochasticChildren()->end(); ++p) 
    {
        if (isInformative(*p, sample_graph)) 
            return true;
    }
    for (set<DeterministicNode*>::iterator p = node->deterministicChildren()->begin();
         p != node->deterministicChildren()->end(); ++p) 
    {
	if (isInformative(*p, sample_graph)) 
	    return true;
    }
    return false;
}

static bool isInformative(DeterministicNode *node, Graph const &sample_graph)
{
    if (!sample_graph.contains(node))
	return false;

    for (StochasticNodeSet::iterator p = node->stochasticChildren()->begin();
         p != node->stochasticChildren()->end(); ++p) 
    {
        if (isInformative(*p, sample_graph)) 
            return true;
    }
    for (set<DeterministicNode*>::iterator p = node->deterministicChildren()->begin();
         p != node->deterministicChildren()->end(); ++p) 
    {
	if (isInformative(*p, sample_graph)) 
	    return true;
    }
    return false;
}

static bool classifyNode(StochasticNode *snode, Graph const &sample_graph, 
			 ConstStochasticNodeSet &sset)
{
    // classification function for stochastic nodes
      
    if (!sample_graph.contains(snode))
	return false;

    if (sset.count(snode))
	return true;

    if (isInformative(snode, sample_graph)) {
	sset.insert(snode);
	return true;
    }
    return false;
}

static bool classifyNode(DeterministicNode *dnode, Graph const &sample_graph,
                         ConstStochasticNodeSet &sset,
			 set<DeterministicNode const *> &dset,
			 vector<DeterministicNode *> &dvec)
{
    //  Recursive classification function for deterministic nodes

    if (!sample_graph.contains(dnode))
	return false;
    
    if (dset.count(dnode))
	return true;
    
    bool informative = false;
    StochasticNodeSet::const_iterator p; 
    for (p = dnode->stochasticChildren()->begin(); 
	 p != dnode->stochasticChildren()->end(); ++p)
    {
	if (classifyNode(*p, sample_graph, sset)) 
	    informative = true;
    }
    set<DeterministicNode*>::const_iterator q;
    for (q = dnode->deterministicChildren()->begin();
	 q != dnode->deterministicChildren()->end(); ++q)
    {
	if (classifyNode(*q, sample_graph, sset, dset, dvec)) 
	    informative = true;
    }
    if (informative) {
	dset.insert(dnode);
	dvec.push_back(dnode);
    }
    return informative;
}


void Sampler::classifyChildren(vector<StochasticNode *> const &nodes,
			       Graph const &graph,
			       vector<StochasticNode const*> &stoch_nodes,
			       vector<DeterministicNode*> &dtrm_nodes)
{
    set<DeterministicNode const *> dset;
    ConstStochasticNodeSet sset;

    dtrm_nodes.clear();

    /* Classify children of each node */
    vector<StochasticNode  *>::const_iterator p; 
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	if (!graph.contains(*p)) {
	    throw logic_error("Sampled node outside of sampling graph");
	}
	StochasticNodeSet const *sch = (*p)->stochasticChildren();
	for (StochasticNodeSet::const_iterator q = sch->begin();
	     q != sch->end(); ++q)
	{
	    classifyNode(*q, graph, sset);
	}
	set<DeterministicNode*> const *dch = (*p)->deterministicChildren();
	for (set<DeterministicNode*>::const_iterator q = dch->begin();
	     q != dch->end(); ++q)
	{
	    classifyNode(*q, graph, sset, dset, dtrm_nodes);
	}
    }

    /* Strip nodes to be sampled out of the set of stochastic
       children. Such nodes would contribute to both the prior
       AND the likelihood, causing incorrect calculation of the
       log full conditional */
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	sset.erase(*p);
    }

    stoch_nodes.clear();
    for (ConstStochasticNodeSet::const_iterator i = sset.begin();
         i != sset.end(); ++i)
    {
       stoch_nodes.push_back(*i);
    }

    // Deterministic nodes are pushed onto dtrm_nodes in reverse order
    reverse(dtrm_nodes.begin(), dtrm_nodes.end());
}

double Sampler::logFullConditional(unsigned int chain) const
{
    double lprior = 0.0;
    vector<StochasticNode*>::const_iterator p = _nodes.begin();
    for (; p != _nodes.end(); ++p) {
	lprior += (*p)->logDensity(chain);
    }
  
    double llike = 0.0;
    vector<StochasticNode const*>::const_iterator q = _stoch_children.begin();
    for (; q != _stoch_children.end(); ++q) {
	llike += (*q)->logDensity(chain);
    }

    double lfc = lprior + llike;
    if(jags_isnan(lfc)) {
	/* 
	   Try to find where the calculation went wrong. At this point,
	   we are definitely going to throw an exception. It's just a
	   question of working out which error message. So we can afford
	   to be laborious.
	*/

	//Check prior
	for (p = _nodes.begin(); p != _nodes.end(); ++p) {
	    if (jags_isnan((*p)->logDensity(chain))) {
		throw NodeError(*p, "Failure to calculate log density");
	    }
	}
	if (jags_isnan(lprior)) {
	    throw runtime_error(string("Failure to calculate prior density in ")
				+ name());
	}

	//Recalculate the deterministic children, checking for
	//invalid values
	vector<DeterministicNode*>::const_iterator r;
	for (r =_determ_children.begin(); r != _determ_children.end(); ++r) {
	    if(!(*r)->checkParentValues(chain)) {
		throw NodeError(*r, "Invalid parent values");
	    }
	    (*r)->deterministicSample(chain);
	}

	//Check likelihood
	for (q = _stoch_children.begin(); q != _stoch_children.end(); ++q) {
	    if (jags_isnan((*q)->logDensity(chain))) {
		throw NodeError(*q, "Failure to calculate log density");
	    }
	}
	if (jags_isnan(llike)) {
	    throw runtime_error(string("Failure to calculate likelihood in ")
				+ name());
	}

	//This could happen adding -Inf to +Inf
	if (!jags_finite(lprior) && !jags_finite(llike)) {
	    throw runtime_error(string("Prior and likelihood are incompatible")
				+ " in " + name());
	}

	//Something else went wrong, but what?
	throw runtime_error(string("Failure to calculate log full conditional")
			    + " in " + name());
    }

    return lfc;
}

double Sampler::logPrior(unsigned int chain) const
{
    double lprior = 0.0;

    vector<StochasticNode*>::const_iterator p = _nodes.begin();
    for (; p != _nodes.end(); ++p) {
	lprior += (*p)->logDensity(chain);
    }
  
    if(jags_isnan(lprior)) {
	//Try to find where the calculation went wrong
	for (p = _nodes.begin(); p != _nodes.end(); ++p) {
	    if (jags_isnan((*p)->logDensity(chain))) {
		throw NodeError(*p, "Failure to calculate log density");
	    }
	}
	throw logic_error("Failure in Sampler::logLikelihood");
    }

    return lprior;
}

double Sampler::logLikelihood(unsigned int chain) const
{
    double llik = 0.0;

    vector<StochasticNode const*>::const_iterator q = _stoch_children.begin();
    for (; q != _stoch_children.end(); ++q) {
	llik += (*q)->logDensity(chain);
    }
  
    if(jags_isnan(llik)) {
	//Try to find where the calculation went wrong
	for (q = _stoch_children.begin(); q != _stoch_children.end(); ++q) {
	    if (jags_isnan((*q)->logDensity(chain))) {
		throw NodeError(*q, "Failure to calculate log density");
	    }
	}

	//This could  happen if we try to add +Inf to -Inf
	throw logic_error("Failure in Sampler::logLikelihood");
    }

    return llik;
}

vector<StochasticNode const*> const &Sampler::stochasticChildren() const
{
  return _stoch_children;
}

vector<DeterministicNode*> const &Sampler::deterministicChildren() const
{
  return _determ_children;
}

void Sampler::setValue(double const * value, unsigned int length,
		       unsigned int chain)
{
    if (length != _length) {
      throw logic_error("Argument length mismatch in Sampler::setValue");
    }

    for (unsigned int i = 0; i < _nodes.size(); ++i) {
	Node *node = _nodes[i];
	node->setValue(value, node->length(), chain);
	value += node->length();
    }

    for (vector<DeterministicNode*>::iterator p(_determ_children.begin());
	 p != _determ_children.end(); ++p) {
      (*p)->deterministicSample(chain);
    }
}

unsigned int Sampler::length() const
{
    return _length;
}

static void stochChildren(Node *node, Graph const &graph,
			  ConstStochasticNodeSet &children)
{
    StochasticNodeSet const *sch = node->stochasticChildren();
    for (StochasticNodeSet::const_iterator p = sch->begin();
	 p != sch->end(); ++p)
    {
	if (graph.contains(*p))
	    children.insert(*p);
    }
    set<DeterministicNode*> const *dch = node->deterministicChildren();
    for (set<DeterministicNode*>::const_iterator p = dch->begin();
	 p != dch->end(); ++p)
    {
	if (graph.contains(*p))
	    stochChildren(*p, graph, children);
    }
}
    
void Sampler::getStochasticChildren(vector<StochasticNode *> const &nodes,
				    Graph const &graph,
				    ConstStochasticNodeSet &children)

{

    vector<StochasticNode  *>::const_iterator p; 
    
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	if (!graph.contains(*p)) {
	    throw logic_error("Sampled node outside of sampling graph");
	}
	stochChildren(*p, graph, children);
    }

    // Strip given nodes out of the set of stochastic children.
    
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	children.erase(*p);
    }
}
