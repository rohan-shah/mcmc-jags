#include <config.h>
#include <model/Model.h>
#include <model/MonitorFactory.h>
#include <model/Monitor.h>
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
using std::copy;

Model::Model(unsigned int nchain)
    : _samplers(0), _nchain(nchain), _rng(nchain, 0), _iteration(0),
      _is_initialized(false), _adapt(false), _data_gen(false)
{
}

Model::~Model()
{
    while(!_samplers.empty()) {
	Sampler *sampler0 = _samplers.back();
	delete sampler0;
	_samplers.pop_back();
    }
    for (list<Monitor*>::const_iterator p = _default_monitors.begin();
	 p != _default_monitors.end(); ++p)
    {
	delete *p;
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

    //Count number of unassigned RNGs
    unsigned int n  = 0;
    for (unsigned int i = 0; i < _nchain; ++i) {
	if (_rng[i] == 0)
	    ++n;
    }

    vector<RNG*> new_rngs;
    for (list<RNGFactory*>::const_iterator p = rngFactories().begin();
	 p != rngFactories().end(); ++p) 
    {
	vector<RNG*> rngs = (*p)->makeRNGs(n);
	for (unsigned int j = 0; j < rngs.size(); ++j) {
	    new_rngs.push_back(rngs[j]);
	}
	if (n == 0)
	    break;
    }
    
    if (n > 0) {
	throw runtime_error("Cannot generate sufficient RNGs");
    }
    else {
	unsigned int j = 0;
	for (unsigned int i = 0; i < _nchain; ++i) {
	    if (_rng[i] == 0) {
		_rng[i] = new_rngs[j++];
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

void Model::initialize(bool datagen)
{
    if (_is_initialized)
	throw logic_error("Model already initialized");

    if (!_graph.isClosed())
	throw runtime_error("Graph not closed");
    /* Redundant
    if (_graph.hasCycle()) 
	throw runtime_error("Directed cycle in graph");
    */

    //Get nodes in forward-sampling order
    vector<Node*> sorted_nodes;
    _graph.getSortedNodes(sorted_nodes);

    // Choose random number generators
    chooseRNGs();

    //Initialize nodes
    initializeNodes(sorted_nodes);

    // Choose Samplers
    chooseSamplers(sorted_nodes);
    
    if (datagen) {
	Graph egraph;
	for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
	     p != _extra_nodes.end(); ++p)
	{
	    egraph.add(*p);
	}
	_sampled_extra.clear();
	egraph.getSortedNodes(_sampled_extra);
	_data_gen = true;
    }

    // Switch to adaptive mode if we find an adaptive sampler
    for (unsigned int i = 0; i < _samplers.size(); ++i) {
	if (_samplers[i]->isAdaptive()) {
	    _adapt = true;
	    break;
	}
    }
    
    _is_initialized = true;
}

void Model::initializeNodes(vector<Node*> const &sorted_nodes)
{
    vector<Node*>::const_iterator i;
    for (i = sorted_nodes.begin(); i != sorted_nodes.end(); ++i) {
	Node *node = *i;
	for (unsigned int n = 0; n < _nchain; ++n) {
	    if (!node->checkParentValues(n)) {
		throw NodeError(node, "Invalid parent values");
	    }
	    if (!node->initialize(_rng[n], n)) {
		throw NodeError(node, "Initialization failure");
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
	    if (asStochastic(node) && node->isRandomVariable()) {
		//FIXME: dynamic casting of non-constant pointer
		stochastic_nodes.insert(dynamic_cast<StochasticNode*>(node));
	    }
	    break;
	case 2:
	    sample_graph.add(node);
	    break;
	}
    }

    set<StochasticNode*> sset = stochastic_nodes;
    // Traverse the list of samplers, selecting nodes that can be sampled
    list<SamplerFactory const *> const &sfactories = samplerFactories();
    for (list<SamplerFactory const *>::const_iterator p = sfactories.begin();
	 p != sfactories.end(); ++p)
    {
	(*p)->makeSampler(sset, sample_graph, _samplers);
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

    stable_sort(_samplers.begin(), _samplers.end(), less_sampler(node_map));
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
	
	for (vector<Sampler*>::iterator i = _samplers.begin(); 
	     i != _samplers.end(); ++i) 
	{
	    (*i)->update(_rng);
	}

	for (unsigned int n = 0; n < _nchain; ++n) {
	    for (vector<Node*>::const_iterator k = _sampled_extra.begin();
		 k != _sampled_extra.end(); ++k)
	    {
                if (!(*k)->checkParentValues(n)) {
                     throw NodeError(*k, "Invalid parent values");
                }
		(*k)->randomSample(_rng[n], n);
	    }
	}
	_iteration++;

	for (list<Monitor*>::iterator k = _monitors.begin(); 
	     k != _monitors.end(); k++) 
	{
	    (*k)->update(_iteration);
	}
    }
}

unsigned int Model::iteration() const
{
  return _iteration;
}

bool Model::adaptOff() 
{
    bool status = true;

    for (vector<Sampler*>::iterator p = _samplers.begin(); 
	 p != _samplers.end(); ++p)
    {
	if (!(*p)->adaptOff())
	    status = false;
    }

    _adapt = false;
    return status;
}

bool Model::isAdapting() const
{
  return _adapt;
}


void Model::setSampledExtra()
{
    /* If a mode is not a data generating model, uninformative nodes
       do not need to be updated, unless they have a descendant that 
       is being monitored. This function finds those nodes and adds
       them to the vector _sampled_extra.
    */
       
    if (_data_gen) {
	// In a data generating model, all uninformative nodes are
	// sampled, so nothing to be done
	return;
    }

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



void Model::addMonitor(Monitor *monitor)
{
    if (_adapt) {
	throw logic_error("Cannot add monitor to adapting model");
    }
    
    _monitors.push_back(monitor);
    setSampledExtra();
}

void Model::removeMonitor(Monitor *monitor)
{
    _monitors.remove(monitor);
    setSampledExtra();
}

void Model::addExtraNode(Node *node)
{
    if (!_is_initialized) {
	throw logic_error("Attempt to add extra node to uninitialized model");
    }
    if (node->isObserved()) {
	throw logic_error("Cannot add observed node to initialized model");
    }
    if (!node->children()->empty()) {
	throw logic_error("Cannot add extra node with children");
    }

    if (_extra_nodes.count(node)) {
	throw logic_error("Extra node already in model");
    }
    
    for (vector<Node const *>::const_iterator p = node->parents().begin(); 
	 p != node->parents().end(); ++p)
    {
	if (!_graph.contains(*p)) {
	throw logic_error("Extra node has parents not in model");
	}
    }

    if (!_graph.contains(node)) {
	_graph.add(node);
    }

    _extra_nodes.insert(node);
    if (_data_gen) {
	//Extra nodes are automatically sampled
	_sampled_extra.push_back(node);
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

list<MonitorFactory *> &Model::monitorFactories()
{
    static list<MonitorFactory *> _monitorfac;
    return _monitorfac;
}

unsigned int Model::nchain() const
{
  return _nchain;
}

RNG *Model::rng(unsigned int chain) const
{
  return _rng[chain];
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
      _rng[chain] = rng;
      return true;
    }
  }

  return false;
}

bool Model::setRNG(RNG *rng, unsigned int chain)
{
  if (chain >= _nchain)
     throw logic_error("Invalid chain number in Model::setRNG");

  _rng[chain] = rng;
  return true;
}

list<Monitor*> const &Model::monitors() const
{
  return _monitors;
}

bool Model::setDefaultMonitors(string const &type, unsigned int thin)
{
    list<MonitorFactory*> const &faclist = monitorFactories();

    for(list<MonitorFactory*>::const_iterator j = faclist.begin();
	j != faclist.end(); ++j)
    {
	vector <Node const *> default_nodes = (*j)->defaultNodes(this, type);
	if (!default_nodes.empty()) {
	    unsigned int start = iteration() + 1;
	    for (unsigned int i = 0; i < default_nodes.size(); ++i) {
		Monitor *monitor = (*j)->getMonitor(default_nodes[i], this,
						    start, thin, type);
		if (!monitor) {
		    throw logic_error("Invalid default monitor");
		}
		addMonitor(monitor);
		/* Model takes ownership of default monitors */
		_default_monitors.push_back(monitor);
	    }
	    return true;
	}
    }
    return false;
}

void Model::clearDefaultMonitors(string const &type)
{
    list<Monitor*> dmonitors = _default_monitors;
    for (list<Monitor*>::const_iterator p = dmonitors.begin();
	 p != dmonitors.end(); ++p) 
    {
	Monitor *monitor = *p;
	if (monitor->type() == type) {
	    _default_monitors.remove(monitor);
	    _monitors.remove(monitor);
	    delete monitor;
	}
    }
    setSampledExtra();
}

