#include <config.h>
#include <model/Model.h>
#include <model/TraceMonitor.h>
#include <sampler/Sampler.h>
#include <sampler/SamplerFactory.h>
#include <rng/RNGFactory.h>
#include <rng/RNG.h>
#include <graph/GraphMarks.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <graph/Node.h>

#include <fstream>
#include <sstream>
#include <set>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <functional>
#include <map>

using std::map;
using std::pair;
using std::binary_function;
using std::sort;
using std::vector;
using std::list;
using std::set;
using std::ofstream;
using std::logic_error;
using std::runtime_error;
using std::string;
using std::ostringstream;

Model::Model(unsigned int nchain)
  : _nchain(nchain), _chain_info(nchain)
{
  for (unsigned int n = 0; n < _nchain; ++n) {
    _chain_info[n].rng = 0;
    _chain_info[n].iteration = 0;
  }
  _is_graph_checked = false;
  _is_initialized = false;
  _adapt = true;
}

Model::~Model()
{
    while(!_chain_info[0].samplers.empty()) {

	/* Delete sampler from chain 0 */

	Sampler *sampler0 = _chain_info[0].samplers.back();
	delete sampler0;
	_chain_info[0].samplers.pop_back();

	/* Delete samplers from other chains. A single sampler may be
	   responsible for updating more than one chain. We need to
	   ensure that we don't delete it twice */

	for (unsigned int n = 1; n < _nchain; ++n) {
    	    vector<Sampler*> samplers_n = _chain_info[n].samplers;
	    Sampler *last_sampler = samplers_n.back();
	    if (last_sampler != sampler0) {
		delete last_sampler;
	    }
	    samplers_n.pop_back();
	}
    }

    for (unsigned int n = 0; n < _nchain; ++n) {
	
	list<TraceMonitor*> &monitors = _chain_info[n].monitors;
	while(!monitors.empty()) {
	    delete monitors.back();
	    monitors.pop_back();
	}

	//RNGs belong to the factory objects and are deleted by them
    }
}

Graph &Model::graph() 
{
  return _graph;
}

bool Model::isInitialized()
{
  return _is_initialized;
}

void Model::chooseRNGs()
{
  /* Assign default RNG objects for any chain that does not
     currently have one */

  list<RNGFactory*>::const_iterator p = rngFactories().begin();
  for (unsigned int n = 0; n < _nchain; ++n) {
    while (_chain_info[n].rng == 0) {
      if (p == rngFactories().end()) {
	ostringstream msg;
	msg << "Cannot generate RNG for chain " << n << ": No further " <<
	  "RNGFactory objects loaded";
	throw runtime_error(msg.str());
      }
      _chain_info[n].rng = (*p)->makeRNG();
      if (_chain_info[n].rng == 0) {
	//This factory cannot generate any more RNGs. Move to the next one.
	++p;
      }
    }
  }
}

void Model::initialize()
{

    if (_is_initialized)
	throw logic_error("Model already initialized");

    if (!_is_graph_checked)
	throw logic_error("Graph not checked yet");

    for (vector<Node*>::iterator i = _nodes.begin(); i != _nodes.end(); i++) {
       if (!(*i)->initialize()) {
          throw NodeError(*i, "Initialization failure");
       } 
    }

    chooseRNGs();

    _is_initialized = true;
}


void Model::checkGraph()
{
  if (_is_graph_checked) {
    throw logic_error("Already checked ability to sample");
  }
  if (!_graph.isClosed())
    throw runtime_error("Graph not closed");
  if (_graph.hasCycle()) 
    throw runtime_error("Directed cycle in graph");
  if (_nodes.empty()) {
    _graph.getSortedNodes(_nodes);
  }

  _is_graph_checked = true;
}

struct less_sampler {  
  /* 
     Comparison operator for Samplers which sorts them according to
     the partial ordering defined by the DAG, using the first node
     in the vector of nodes sampled by the sampler
  */
  map<Node const*, int> const &_node_map;

  less_sampler(map<Node const*, int> const &node_map) : _node_map(node_map) {};
  
  bool operator()(Sampler *x, Sampler *y) const {
    int indx = _node_map.find(x->nodes()[0])->second;
    int indy = _node_map.find(y->nodes()[0])->second;
    //return indx < indy; //Forward sampling order
    return indx > indy; //Backward sampling order
  };
};

void Model::chooseSamplers()
{
  if (!_is_graph_checked) 
    throw logic_error("Graph not checked");

  // Mark observed nodes
  GraphMarks marks(_graph);
  vector<Node*>::reverse_iterator i = _nodes.rbegin();
  for (; i != _nodes.rend(); ++i) {
      if ((*i)->isObserved()) {
	  marks.mark(*i,2);
      }
  }
    
  // Now mark ancestors of observed nodes
  for (i = _nodes.rbegin(); i != _nodes.rend(); ++i) {
    if (marks.mark(*i) != 2) {
      for (set<Node*>::const_iterator ch = (*i)->children()->begin(); 
	   ch != (*i)->children()->end(); ++ch) 
	{
	  if (marks.mark(*ch) != 0) {
	    marks.mark(*i,1);
	    break;
	  }
	}
    }
  }

  // Create set of unobserved stochastic nodes, for which we need
  // to find a sampler,  a graph within which sampling will take
  // place (excluding uninformative nodes), and a set of "extra"
  // uninformative nodes that will be updated by the model at the
  // end of every iteration.
  set<StochasticNode*> stochastic_nodes;
  Graph graph;
  for (vector<Node*>::iterator i = _nodes.begin(); i != _nodes.end(); i++) {
    switch(marks.mark(*i)) {
    case 0:
      _extra_nodes.insert(*i);
      break;
    case 1:
      graph.add(*i);
      if (asStochastic(*i)) {
	//FIXME: dynamic casting of non-constant pointer
	stochastic_nodes.insert(dynamic_cast<StochasticNode*>(*i));
      }
      break;
    case 2:
      graph.add(*i);
      break;
    }
  }

  vector <vector<Sampler*> > sam(nchain());

  set<StochasticNode*> sset = stochastic_nodes;
  // Traverse the list of samplers, selecting nodes that can be sampled
  list<SamplerFactory const *> const &sfactories = samplerFactories();
  for (list<SamplerFactory const *>::const_iterator p = sfactories.begin();
       p != sfactories.end(); ++p)
    {
	(*p)->makeSampler(sset, graph, sam);
    }
  
  // Make sure we found a sampler for all the nodes
  if (!sset.empty()) {
      
      throw NodeError(*sset.begin(),
		      "Unable to find appropriate sampler");
  }
  
  /**
   * Now sort the samplers in order
   *
   * The map node_map associates each node in the graph with its index
   * in the vector of sorted nodes. This is used by the comparison
   * operator less_sampler.
   */
  static map <Node const *, int> node_map;
  int index = 0;
  for (vector<Node*>::iterator i = _nodes.begin(); i != _nodes.end(); i++)
    {
      node_map.insert(pair<Node const*, int>(*i, index));
      index++;
    }

  for (unsigned int n = 0; n < nchain(); ++n) {
    _chain_info[n].samplers = sam[n];
    stable_sort(_chain_info[n].samplers.begin(), 
		_chain_info[n].samplers.end(), less_sampler(node_map));
  }

  _can_sample = true;
}

void Model::update(unsigned int niter)
{
    if (!_is_initialized) {
	throw logic_error("Attempt to update uninitialized model");
    }
    if (!_can_sample) {
        throw logic_error("Attempt to update model with no samplers");
    }


    for (unsigned int iter = 0; iter < niter; ++iter) {    
	
	for (unsigned int n = 0; n < _nchain; ++n) {
	    
	    vector<Sampler*> &samplers = _chain_info[n].samplers;
	    list<TraceMonitor*> &monitors = _chain_info[n].monitors;
	    RNG *rng = _chain_info[n].rng;
	    
	    for (vector<Sampler*>::iterator i = samplers.begin(); 
		 i != samplers.end(); ++i) 
	    {
	      (*i)->update(rng);
	    }
	    for (vector<Node*>::const_iterator k = _sampled_extra.begin();
		 k != _sampled_extra.end(); ++k)
	    {
		(*k)->randomSample(rng, n);
	    }
	    _chain_info[n].iteration++;
	    for (list<TraceMonitor*>::iterator k = monitors.begin(); 
		 k != monitors.end(); k++) 
	    {
		(*k)->update(_chain_info[n].iteration, n);
	    }
	}
    }
}

unsigned int Model::iteration(unsigned int chain) const
{
  return _chain_info[chain].iteration;
}

void Model::adaptOff() 
{
  for (unsigned int n = 0; n < _nchain; ++n) {
    for (vector<Sampler*>::iterator p = _chain_info[n].samplers.begin();
	 p != _chain_info[n].samplers.end(); ++p)
      {
	(*p)->adaptOff();
      }
  }
  _adapt = false;
}

void Model::setMonitor(Node *node, int thin)
{

  if (_adapt) {
    adaptOff();
  }
  
  if (_monitored_nodes.count(node)) {
    //Nothing to do. Node is already being monitored.
    return;
  }

  if(!_graph.contains(node)) {
    addExtraNode(node);
  }

  for (unsigned int n = 0; n < _nchain; ++n) {
     TraceMonitor *monitor = new TraceMonitor(node, iteration(n) + 1, thin);
     _chain_info[n].monitors.push_back(monitor);
  }   
  _monitored_nodes.insert(node);

  // Recalculate the vector of uninformative nodes that need sampling

  //Insert extra nodes into a new graph
  Graph egraph;
  for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
       p != _extra_nodes.end(); ++p)
      {
	  egraph.add(*p);
      }
  //Mark the ancestors of all monitored nodes in this graph
  GraphMarks emarks(egraph);
  for (set<Node*>::const_iterator j = _monitored_nodes.begin();
       j != _monitored_nodes.end(); ++j)
    {
        if (egraph.contains(*j)) {
            emarks.mark(*j,1);
	    emarks.markAncestors(*j, 1);
        }
    }
  //Remove unmarked nodes from graph
  for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
       p != _extra_nodes.end(); ++p)
      {
	  if (emarks.mark(*p) == 0)
	      egraph.remove(*p);
      }
  //Replace vector of sampled extra nodes
  _sampled_extra.clear();
  egraph.getSortedNodes(_sampled_extra);
}

void Model::clearMonitor(Node *node)
{
  for (unsigned int n = 0; n < _nchain; ++n) {
    list<TraceMonitor*> &monitors = _chain_info[n].monitors;
    for (list<TraceMonitor*>::iterator j = monitors.begin();
         j != monitors.end(); j++) 
      {
        if ((*j)->node() == node) {
          list<TraceMonitor*>::iterator k = j++;
	  monitors.erase(k);
        }
      }
  }

  _monitored_nodes.erase(node);
}

list<TraceMonitor*> const &Model::monitors(unsigned int chain) const
{
  return _chain_info[chain].monitors;
}

void Model::addExtraNode(Node *node)
{
  if (!_is_initialized) 
    throw logic_error("Attempt to add extra node to uninitialized model");
  if (node->isObserved()) {
    throw logic_error("Cannot add observed node to initialized model");
  }
  if (!node->children()->empty()) {
    throw logic_error("Cannot add extra node with children");
  }
  for (vector<Node const *>::const_iterator p = node->parents().begin(); 
       p != node->parents().end(); ++p)
    {
      if (!_graph.contains(*p)) {
	throw logic_error("Extra node has parents not in model");
      }
    }

  if (_extra_nodes.count(node)) {
    throw logic_error("Extra node already in model");
  }
  else {
     _extra_nodes.insert(node);
  }
  if (!_graph.contains(node)) {
     _graph.add(node);
  }

}

list<SamplerFactory const *> &Model::samplerFactories()
{
  static list<SamplerFactory const *> _samplers;
  return _samplers;
}

list<RNGFactory *> &Model::rngFactories()
{
  static list<RNGFactory *> _rngfac;
  return _rngfac;
}

unsigned int Model::nchain() const
{
  return _nchain;
}

bool Model::canSample() const
{
  return _can_sample;
}

RNG *Model::rng(unsigned int chain) const
{
  return _chain_info[chain].rng;
}

bool Model::setRNG(string const &name, unsigned int chain)
{
  /* Assign default RNG objects for any chain that does not
     currently have one */

  if (chain >= _nchain)
     throw logic_error("Invalid chain number in Model::setRNG");

  list<RNGFactory*>::const_iterator p;
  for (p = rngFactories().begin(); p != rngFactories().end(); ++p) {
    RNG *rng = (*p)->makeRNG(name);
    if (rng) {
      _chain_info[chain].rng = rng;
      return true;
    }
  }

  return false;
}

bool Model::setRNG(RNG *rng, unsigned int chain)
{
  if (chain >= _nchain)
     throw logic_error("Invalid chain number in Model::setRNG");

  _chain_info[chain].rng = rng;
  return true;
}

