#include <config.h>
#include <sampler/Sampler.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>
#include <sarray/nainf.h>

#include <stdexcept>
#include <set>
#include <string>
#include <cmath>

using std::vector;
using std::set;
using std::runtime_error;
using std::logic_error;
using std::string;

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
    : _length(sumLength(nodes)), _nodes(nodes)
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

static void classifyNode(Node *node, Graph const &sample_graph, 
			 Graph &sgraph, Graph &dgraph)
{
    if (!sample_graph.contains(node))
	return;

    if (node->isVariable() && asStochastic(node)) {
	sgraph.add(node);
    }
    else if (!dgraph.contains(node)) {
	dgraph.add(node);      
	for (set<Node*>::iterator p = node->children()->begin();
	     p != node->children()->end(); ++p) {
	    classifyNode(*p, sample_graph, sgraph, dgraph);
	}
    }
}

void Sampler::classifyChildren(vector<StochasticNode *> const &nodes,
			       Graph const &graph,
			       vector<StochasticNode const*> &stoch_nodes,
			       vector<DeterministicNode*> &dtrm_nodes)
{
  Graph dgraph, sgraph;

  /* Classify children of each node */
  vector<StochasticNode  *>::const_iterator p = nodes.begin();
  for (; p != nodes.end(); ++p) {
    StochasticNode *snode = *p;
    if (!graph.contains(snode)) {
      throw logic_error("Sampled node outside of sampling graph");
    }
    for (set<Node*>::const_iterator q = snode->children()->begin(); 
	 q != snode->children()->end(); ++q) 
      {
	classifyNode(*q, graph, sgraph, dgraph);
      }
  }

  /* Strip nodes to be sampled out of the graph of stochastic
     children. Such nodes would contribute to both the prior
     AND the likelihood, causing incorrect calculation of the
     log full conditional */
  for (p = nodes.begin(); p != nodes.end(); ++p) {
    sgraph.remove(*p);
  }

  vector<Node*> svector;
  sgraph.getNodes(svector);
  for (vector<Node*>::iterator i = svector.begin(); i != svector.end(); 
       ++i) 
    {
      stoch_nodes.push_back(asStochastic(*i));
    }
  
  vector<Node*> dvector;
  dgraph.getSortedNodes(dvector);
  for (vector<Node*>::iterator i = dvector.begin(); i != dvector.end();
       ++i)
    {
      dtrm_nodes.push_back(dynamic_cast<DeterministicNode*>(*i));
    }
}

double Sampler::logFullConditional(unsigned int chain) const
{
  double lfc = 0.0;

  vector<StochasticNode*>::const_iterator p = _nodes.begin();
  for (; p != _nodes.end(); ++p) {
    lfc += (*p)->logDensity(chain);
  }
  
  vector<StochasticNode const*>::const_iterator q = _stoch_children.begin();
  for (; q != _stoch_children.end(); ++q) {
    lfc += (*q)->logDensity(chain);
  }
  
  if(jags_isnan(lfc)) {
    //Try to find where the calculation went wrong
    for (p = _nodes.begin(); p != _nodes.end(); ++p) {
      if (jags_isnan((*p)->logDensity(chain))) {
	throw NodeError(*p, "Failure to calculate log density");
      }
    }
    
    for (q = _stoch_children.begin(); q != _stoch_children.end(); ++q) {
      if (jags_isnan((*q)->logDensity(chain))) {
	throw NodeError(*q, "Failure to calculate log density");
      }
    }

    //This could  happen if we try to add +Inf to -Inf
    throw logic_error("Failure in Sampler::logFullConditional");
  }

  return lfc;
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
	StochasticNode *snode = _nodes[i];
	snode->setValue(value, snode->length(), chain);
	value += snode->length();
    }

    for (vector<DeterministicNode*>::iterator p(_determ_children.begin());
	 p != _determ_children.end(); ++p) {
      (*p)->deterministicSample(chain);
    }
}

