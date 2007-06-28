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
using std::stable_sort;

Model::Model(unsigned int nchain)
  : _nchain(nchain), _chain_info(nchain), _is_initialized(false), _adapt(true)
{
  for (unsigned int n = 0; n < _nchain; ++n) {
    _chain_info[n].rng = 0;
    _chain_info[n].iteration = 0;
  }
}

Model::~Model()
{
    while(!_chain_info[0].samplers.empty()) {

	/* Delete samplers from other chains. A single sampler may be
	   responsible for updating more than one chain. We need to
	   ensure that we don't delete it twice */

	Sampler *sampler0 = _chain_info[0].samplers.back();

	for (unsigned int n = 1; n < _nchain; ++n) {
	    Sampler *last_sampler = _chain_info[n].samplers.back();
	    if (last_sampler != sampler0) {
		delete last_sampler;
	    }
	    _chain_info[n].samplers.pop_back();
	}

	/* Delete sampler from chain 0 */
	delete sampler0;
	_chain_info[0].samplers.pop_back();

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
		msg << "Cannot generate RNG for chain " << n
		    << ": No further RNGFactory objects loaded";
		throw runtime_error(msg.str());
	    }
	    _chain_info[n].rng = (*p)->makeRNG();
	    if (_chain_info[n].rng == 0) {
		//This factory cannot generate any more RNGs. 
		// Move to the next one.
		++p;
	    }
	}
    }
}

/*
static Node *checkDataGen(vector<Node*> const &nodes)
{
    // A data generating model is valid if there are no observed
    //   nodes with unobserved parents 
    
    //FIXME: Not using the fact that vector of nodes is sorted

    for (unsigned int i = 0; i < nodes.size(); ++i) {
	if (nodes[i]->isObserved()) {
	    vector<Node const*> const &parents = nodes[i]->parents();
	    for(vector<Node const*>::const_iterator p = parents.begin();
		p != parents.end(); ++p) 
	    {
		if (!((*p)->isObserved())) {
		    return nodes[i]; //Return invalid node
		}
	    }
	}
    }
    return 0;
}
*/

void Model::initialize(bool random)
{
    if (_is_initialized)
	throw logic_error("Model already initialized");

    if (!_graph.isClosed())
	throw runtime_error("Graph not closed");
    if (_graph.hasCycle()) 
	throw runtime_error("Directed cycle in graph");
    
    //Get nodes in forward-sampling order
    vector<Node*> sorted_nodes;
    _graph.getSortedNodes(sorted_nodes);

    // Choose random number generators
    chooseRNGs();

    //Initialize nodes
    initializeNodes(sorted_nodes, random);

    // Choose Samplers
    chooseSamplers(sorted_nodes);

    _is_initialized = true;
}

void Model::initializeNodes(vector<Node*> const &sorted_nodes,
			    bool random)
{
    vector<Node*>::const_iterator i;
    for (i = sorted_nodes.begin(); i != sorted_nodes.end(); i++) {
	Node *node = *i;
	for (unsigned int n = 0; n < _nchain; ++n) {
	    if (!node->checkParentValues(n)) {
		throw NodeError(node, "Invalid parent values");
	    }
	}
	if (!node->initialize()) {
	    throw NodeError(node, "Initialization failure");
	} 
	if (random && !node->isObserved()) {
	    for (unsigned int ch = 0; ch < _nchain; ++ch) {
		node->randomSample(_chain_info[ch].rng, ch);
	    }
	}
    }
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


void Model::chooseSamplers(vector<Node*> const &sorted_nodes)
{
    /*
     * Selects samplers. For each chain, samplers are selected by
     * traversing the list of SamplerFactories in order. If there are
     * any informative stochastic nodes left without samplers after all
     * factories have been tried, then a runtime error is thrown
     *
     * @see Model#samplerFactories
     */

    // Mark observed nodes
    GraphMarks marks(_graph);
    vector<Node*>::const_reverse_iterator i = sorted_nodes.rbegin();
    for (; i != sorted_nodes.rend(); ++i) {
	if ((*i)->isObserved()) {
	    marks.mark(*i,2);
	}
    }
    
    // Now mark ancestors of observed nodes
    for (i = sorted_nodes.rbegin(); i != sorted_nodes.rend(); ++i) {
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
    Graph sample_graph;
    vector<Node*>::const_iterator j;
    for (j = sorted_nodes.begin(); j != sorted_nodes.end(); j++) {
	Node *node = *j;
	switch(marks.mark(node)) {
	case 0:
	    _extra_nodes.insert(node);
	    break;
	case 1:
	    sample_graph.add(node);
	    if (asStochastic(node)) {
		//FIXME: dynamic casting of non-constant pointer
		stochastic_nodes.insert(dynamic_cast<StochasticNode*>(node));
	    }
	    break;
	case 2:
	    sample_graph.add(node);
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
	(*p)->makeSampler(sset, sample_graph, sam);
    }
  
    // Make sure we found a sampler for all the nodes
    if (!sset.empty()) {
      
	throw NodeError(*sset.begin(),
			"Unable to find appropriate sampler");
    }
  
    // Now sort the samplers in order
    //
    // The map node_map associates each node in the graph with its index
    // in the vector of sorted nodes. This is used by the comparison
    // operator less_sampler.
    static map <Node const *, int> node_map;
    int index = 0;
    for (j = sorted_nodes.begin(); j != sorted_nodes.end(); j++)
    {
	node_map.insert(pair<Node const*, int>(*j, index));
	index++;
    }

    for (unsigned int n = 0; n < nchain(); ++n) {
	_chain_info[n].samplers = sam[n];
	stable_sort(_chain_info[n].samplers.begin(), 
		    _chain_info[n].samplers.end(), less_sampler(node_map));
    }
}

void Model::update(unsigned int niter)
{
    if (!_is_initialized) {
	throw logic_error("Attempt to update uninitialized model");
    }

    list<Monitor*>::const_iterator p;
    for (p = _monitors.begin(); p != _monitors.end(); ++p) {
	(*p)->reserve(niter);
    }

    for (unsigned int iter = 0; iter < niter; ++iter) {    
	
	for (unsigned int n = 0; n < _nchain; ++n) {
	    
	    vector<Sampler*> &samplers = _chain_info[n].samplers;
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
	    for (list<Monitor*>::iterator k = _monitors.begin(); 
		 k != _monitors.end(); k++) 
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

bool Model::adaptOff() 
{
  bool status = true;

  for (unsigned int n = 0; n < _nchain; ++n) {
    for (vector<Sampler*>::iterator p = _chain_info[n].samplers.begin();
	 p != _chain_info[n].samplers.end(); ++p)
      {
	if(!(*p)->adaptOff())
           status = false;
      }
  }
  _adapt = false;
  return status;
}

bool Model::isAdapting() const
{
  return _adapt;
}

void Model::addMonitor(Monitor *monitor)
{

    if (_adapt) {
	adaptOff(); //FIXME: Efficiency test ignored
    }
    
    // FIXME: Need to do this where the new node is generated
    /*
    if(!_graph.contains(node)) {
	addExtraNode(node);
    }
    */

    _monitors.push_back(monitor);

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
    for (list<Monitor*>::const_iterator p = _monitors.begin();
	 p != _monitors.end(); ++p)
    {
	Node const *node = (*p)->node();
        if (egraph.contains(node)) {
            emarks.mark(node, 1);
	    emarks.markAncestors(node, 1);
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

void Model::removeMonitor(Monitor *monitor)
{
    _monitors.remove(monitor);
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

bool isSynchronized(Model const *model)
{
    for (unsigned int ch = 1; ch < model->nchain(); ++ch) {
        if (model->iteration(ch) != model->iteration(0)) {
	    return false;
        }
    }
    return true;
}
